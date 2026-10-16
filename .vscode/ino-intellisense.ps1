# Writes build/ino_compile_commands.json so the C/C++ extension can parse
# tf02_http.ino with the same flags arduino-cli uses for the sketch.
#
# arduino-cli's own compile_commands.json only lists the generated
# build/sketch/*.ino.cpp, and hides most flags in @response files and
# -iprefix/-iwithprefixbefore pairs. This expands them into plain -I/-D.

$ErrorActionPreference = 'Stop'
$root   = Split-Path $PSScriptRoot -Parent
$cli    = Join-Path $env:LOCALAPPDATA 'Programs\arduino-ide\resources\app\lib\backend\resources\arduino-cli.exe'
$sketch = Join-Path $root 'tf02_http'
$ino    = Join-Path $sketch 'tf02_http.ino'
$build  = Join-Path $root 'build'

& $cli compile --only-compilation-database --build-path $build $sketch
if ($LASTEXITCODE -ne 0) { throw "arduino-cli failed ($LASTEXITCODE)" }

$db    = Get-Content (Join-Path $build 'compile_commands.json') -Raw | ConvertFrom-Json
$entry = $db | Where-Object { $_.file -like '*tf02_http.ino.cpp' } | Select-Object -First 1
if (-not $entry) { throw 'sketch entry not found in compile_commands.json' }

function Expand-Args([string[]]$list) {
  foreach ($a in $list) {
    if ($a.StartsWith('@')) {
      $text = Get-Content $a.Substring(1) -Raw
      if ($text) { Expand-Args (($text.Trim() -split '\s+') | ForEach-Object { $_ -replace '\\"', '"' }) }
    } else { $a }
  }
}

$out = New-Object System.Collections.Generic.List[string]
$prefix = ''
$flat = @(Expand-Args $entry.arguments)
for ($i = 0; $i -lt $flat.Count; $i++) {
  $a = $flat[$i]
  switch -CaseSensitive ($a) {
    '-iprefix'           { $prefix = $flat[++$i]; continue }
    '-iwithprefixbefore' { $out.Add('-I' + $prefix + $flat[++$i]); continue }
    '-o'                 { $i++; continue }
    { $_ -in '-c', '-MMD' -or $_ -eq $entry.file } { continue }
    default              { $out.Add($a) }
  }
}
$out.AddRange([string[]]@('-include', 'Arduino.h', $ino))

$json = ConvertTo-Json -Depth 4 @(@{ directory = $root; file = $ino; arguments = $out.ToArray() })
Set-Content -Path (Join-Path $build 'ino_compile_commands.json') -Value $json -Encoding UTF8
Write-Host "IntelliSense database written ($($out.Count) args)."
