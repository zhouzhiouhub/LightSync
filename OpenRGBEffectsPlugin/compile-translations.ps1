# Script to compile .ts files to .qm and embed them in resources.qrc
# Usage: Run this script from OpenRGBEffectsPlugin directory

$ErrorActionPreference = "Stop"
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptPath

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "Translation Compiler for Effects Plugin" -ForegroundColor Cyan
Write-Host "======================================`n" -ForegroundColor Cyan

# Find lrelease
$lreleasePaths = @(
    "C:\Qt\5.15.2\msvc2019_64\bin\lrelease.exe",
    "C:\Qt\5.15.2\mingw81_64\bin\lrelease.exe",
    "C:\Qt\5.15.2\msvc2015_64\bin\lrelease.exe",
    "C:\Qt\6.7.0\msvc2019_64\bin\lrelease.exe",
    "C:\Qt\6.8.0\msvc2019_64\bin\lrelease.exe"
)

$lrelease = $null
foreach ($path in $lreleasePaths) {
    if (Test-Path $path) {
        $lrelease = $path
        Write-Host "Found lrelease: $lrelease" -ForegroundColor Green
        break
    }
}

if (-not $lrelease) {
    Write-Host "ERROR: lrelease not found!" -ForegroundColor Red
    Write-Host "Searching in PATH..." -ForegroundColor Yellow
    $lrelease = Get-Command lrelease -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source
    if ($lrelease) {
        Write-Host "Found in PATH: $lrelease" -ForegroundColor Green
    } else {
        Write-Host "Please install Qt or add Qt bin directory to PATH" -ForegroundColor Red
        exit 1
    }
}

# Compile .ts files to .qm
Write-Host "`nCompiling translation files..." -ForegroundColor Yellow
Set-Location "i18n"

$tsFiles = Get-ChildItem -Filter "*.ts"
$compiled = 0

foreach ($tsFile in $tsFiles) {
    $qmFile = $tsFile.FullName -replace '\.ts$', '.qm'
    Write-Host "  $($tsFile.Name) -> $([System.IO.Path]::GetFileName($qmFile))"
    
    & $lrelease $tsFile.FullName -qm $qmFile -silent
    
    if ($LASTEXITCODE -eq 0) {
        $compiled++
    } else {
        Write-Host "    WARNING: Failed to compile $($tsFile.Name)" -ForegroundColor Yellow
    }
}

Set-Location ".."
Write-Host "`nCompiled $compiled translation files" -ForegroundColor Green

# Update resources.qrc
Write-Host "`nUpdating resources.qrc..." -ForegroundColor Yellow

$qrcPath = "resources.qrc"
[xml]$qrc = Get-Content $qrcPath

# Find or create i18n qresource node
$i18nResource = $qrc.RCC.qresource | Where-Object { $_.prefix -eq "/i18n" }

if (-not $i18nResource) {
    Write-Host "  Creating new <qresource prefix='/i18n'> section" -ForegroundColor Cyan
    $i18nResource = $qrc.CreateElement("qresource")
    $i18nResource.SetAttribute("prefix", "/i18n")
    $null = $qrc.RCC.AppendChild($i18nResource)
}

# Clear existing .qm files
$existingFiles = @($i18nResource.file)
foreach ($file in $existingFiles) {
    if ($file -like "*\.qm") {
        $null = $i18nResource.RemoveChild($file)
    }
}

# Add all .qm files with alias
$qmFiles = Get-ChildItem "i18n" -Filter "*.qm"
foreach ($qmFile in $qmFiles) {
    $fileNode = $qrc.CreateElement("file")
    $fileNode.SetAttribute("alias", $qmFile.Name)
    $fileNode.InnerText = "i18n/$($qmFile.Name)"
    $null = $i18nResource.AppendChild($fileNode)
    Write-Host "  Added: i18n/$($qmFile.Name) as :/i18n/$($qmFile.Name)" -ForegroundColor Green
}

# Save with proper formatting
$settings = New-Object System.Xml.XmlWriterSettings
$settings.Indent = $true
$settings.IndentChars = "    "
$settings.NewLineChars = "`n"
$settings.Encoding = [System.Text.UTF8Encoding]::new($false) # UTF-8 without BOM

$writer = [System.Xml.XmlWriter]::Create($qrcPath, $settings)
$qrc.Save($writer)
$writer.Close()

Write-Host "`nresources.qrc updated successfully!" -ForegroundColor Green
Write-Host "`nPlease rebuild the plugin to apply translation changes.`n" -ForegroundColor Cyan

