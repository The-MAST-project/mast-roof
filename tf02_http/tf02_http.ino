// TF02-Pro LiDAR -> ESP32 Ethernet -> HTTP/JSON
// Arduino-ESP32 core 3.x (ETH.begin signature changed from 2.x)
//
// GET /          -> latest reading as JSON
// GET /health    -> link/IP/uptime as JSON

#include <ETH.h>
#include <WebServer.h>

// ---------- pick your board ----------
#define BOARD_OLIMEX_ESP32_POE2
// #define BOARD_OLIMEX_ESP32_POE
// #define BOARD_WT32_ETH01

#if defined(BOARD_OLIMEX_ESP32_POE2)
  // WROVER module: GPIO16/17 belong to PSRAM, so the Ethernet clock is on GPIO0.
  // Arduino IDE: board "OLIMEX ESP32-PoE", PSRAM enabled.
  // TF02-Pro power: 12V header output (jumper set to 12V), GND common.
  #define PHY_ADDR   0
  #define PHY_POWER  12
  #define PHY_MDC    23
  #define PHY_MDIO   18
  #define PHY_CLK    ETH_CLOCK_GPIO0_OUT
  #define TF_RX_PIN  36   // input-only pin, fine for RX
  #define TF_TX_PIN  4
#elif defined(BOARD_OLIMEX_ESP32_POE)
  #define PHY_ADDR   0
  #define PHY_POWER  12
  #define PHY_MDC    23
  #define PHY_MDIO   18
  #define PHY_CLK    ETH_CLOCK_GPIO17_OUT
  #define TF_RX_PIN  36   // input-only pin, fine for RX
  #define TF_TX_PIN  4
#elif defined(BOARD_WT32_ETH01)
  #define PHY_ADDR   1
  #define PHY_POWER  16
  #define PHY_MDC    23
  #define PHY_MDIO   18
  #define PHY_CLK    ETH_CLOCK_GPIO0_IN
  #define TF_RX_PIN  5    // labelled RXD on the header
  #define TF_TX_PIN  17   // labelled TXD on the header
#endif

// ---------- sensor settings ----------
#define TF_BAUD        115200
#define TF_FRAME_HZ    20      // sensor output rate; 100 is factory default
#define STALE_MS       500     // reading considered stale after this
#define MIN_STRENGTH   100     // below this the distance is unreliable

HardwareSerial TF(2);
WebServer server(80);

struct Reading {
  uint16_t dist_cm   = 0;
  uint16_t strength  = 0;
  float    temp_c    = 0;
  uint32_t t_ms      = 0;
  bool     have      = false;
} last;

uint32_t frames_ok = 0, frames_bad = 0;
bool eth_up = false;

// ---------- sensor ----------
void tfSendCmd(const uint8_t *body, size_t n) {
  // body excludes checksum; checksum = low byte of sum of all preceding bytes
  uint8_t sum = 0;
  for (size_t i = 0; i < n; i++) sum += body[i];
  TF.write(body, n);
  TF.write(sum);
  TF.flush();
}

void tfSetFrameRate(uint16_t hz) {
  uint8_t cmd[] = {0x5A, 0x06, 0x03, (uint8_t)(hz & 0xFF), (uint8_t)(hz >> 8)};
  tfSendCmd(cmd, sizeof(cmd));
  delay(50);
  uint8_t save[] = {0x5A, 0x04, 0x11};   // save settings
  tfSendCmd(save, sizeof(save));
  delay(50);
}

void tfPoll() {
  static uint8_t buf[9];
  static uint8_t idx = 0;

  while (TF.available()) {
    uint8_t b = TF.read();

    // sync on 0x59 0x59
    if (idx < 2) {
      if (b == 0x59) buf[idx++] = b;
      else idx = 0;
      continue;
    }
    buf[idx++] = b;
    if (idx < 9) continue;
    idx = 0;

    uint8_t sum = 0;
    for (int i = 0; i < 8; i++) sum += buf[i];
    if (sum != buf[8]) { frames_bad++; continue; }

    frames_ok++;
    last.dist_cm  = buf[2] | (buf[3] << 8);
    last.strength = buf[4] | (buf[5] << 8);
    uint16_t traw = buf[6] | (buf[7] << 8);
    last.temp_c   = traw / 8.0f - 256.0f;
    last.t_ms     = millis();
    last.have     = true;
  }
}

// ---------- http ----------
void sendJson(int code, const String &body) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "no-store");
  server.send(code, "application/json", body);
}

void handleRoot() {
  uint32_t now = millis();
  uint32_t age = last.have ? now - last.t_ms : 0;
  bool stale = !last.have || age > STALE_MS;
  bool valid = !stale && last.strength >= MIN_STRENGTH && last.strength != 65535;

  char out[256];
  snprintf(out, sizeof(out),
    "{\"distance_cm\":%u,\"strength\":%u,\"temp_c\":%.1f,"
    "\"valid\":%s,\"stale\":%s,\"age_ms\":%lu,"
    "\"frames_ok\":%lu,\"frames_bad\":%lu}",
    last.dist_cm, last.strength, last.temp_c,
    valid ? "true" : "false", stale ? "true" : "false",
    (unsigned long)age, (unsigned long)frames_ok, (unsigned long)frames_bad);
  sendJson(stale ? 503 : 200, out);
}

void handleHealth() {
  char out[160];
  snprintf(out, sizeof(out),
    "{\"eth_up\":%s,\"ip\":\"%s\",\"uptime_s\":%lu}",
    eth_up ? "true" : "false", ETH.localIP().toString().c_str(),
    (unsigned long)(millis() / 1000));
  sendJson(200, out);
}

// ---------- ethernet ----------
void onNetEvent(arduino_event_id_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      ETH.setHostname("tf02");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      eth_up = true;
      Serial.printf("ETH up, IP %s\n", ETH.localIP().toString().c_str());
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
    case ARDUINO_EVENT_ETH_STOP:
      eth_up = false;
      Serial.println("ETH down");
      break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);

  TF.begin(TF_BAUD, SERIAL_8N1, TF_RX_PIN, TF_TX_PIN);
  delay(200);
  tfSetFrameRate(TF_FRAME_HZ);

  Network.onEvent(onNetEvent);
  ETH.begin(ETH_PHY_LAN8720, PHY_ADDR, PHY_MDC, PHY_MDIO, PHY_POWER, PHY_CLK);

  server.on("/", handleRoot);
  server.on("/health", handleHealth);
  server.onNotFound([]() { sendJson(404, "{\"error\":\"not found\"}"); });
  server.begin();
}

void loop() {
  tfPoll();
  server.handleClient();
}
