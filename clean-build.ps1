# LightSync 构建清理脚本
# 此脚本用于清理所有构建生成的文件

Write-Host "开始清理 LightSync 构建文件..." -ForegroundColor Green

# 清理旧的构建目录
$oldBuildDirs = @(
    "build\Desktop_Qt_5_15_2_MSVC2019_64bit-Debug\OpenRGB",
    "build\Desktop_Qt_5_15_2_MSVC2019_64bit-Debug\OpenRGBEffectsPlugin",
    "build\Desktop_Qt_5_15_2_MSVC2019_64bit-Debug\LightSync"
)

foreach ($dir in $oldBuildDirs) {
    if (Test-Path $dir) {
        Write-Host "删除: $dir" -ForegroundColor Yellow
        Remove-Item -Path $dir -Recurse -Force -ErrorAction SilentlyContinue
    }
}

# 清理新的构建目录中的中间文件
$newBuildDirs = @(
    "build\LightSync\_intermediate_debug",
    "build\LightSync\_intermediate_release",
    "build\LightSync\plugins\_intermediate_debug",
    "build\LightSync\plugins\_intermediate_release"
)

foreach ($dir in $newBuildDirs) {
    if (Test-Path $dir) {
        Write-Host "删除中间文件: $dir" -ForegroundColor Yellow
        Remove-Item -Path $dir -Recurse -Force -ErrorAction SilentlyContinue
    }
}

# 清理输出目录
$outputDirs = @(
    "out\LightSync\debug",
    "out\LightSync\release"
)

foreach ($dir in $outputDirs) {
    if (Test-Path $dir) {
        Write-Host "清理输出: $dir" -ForegroundColor Yellow
        Remove-Item -Path $dir -Recurse -Force -ErrorAction SilentlyContinue
    }
}

# 清理 Makefile
$makefiles = Get-ChildItem -Path "." -Filter "Makefile*" -Recurse -ErrorAction SilentlyContinue | Where-Object { $_.FullName -notlike "*\OpenRGB\*" -and $_.FullName -notlike "*\OpenRGBEffectsPlugin\*" -and $_.FullName -notlike "*\dependencies\*" }

foreach ($file in $makefiles) {
    Write-Host "删除 Makefile: $($file.FullName)" -ForegroundColor Yellow
    Remove-Item -Path $file.FullName -Force -ErrorAction SilentlyContinue
}

Write-Host "`n清理完成！" -ForegroundColor Green
Write-Host "现在可以重新构建项目了。" -ForegroundColor Cyan

