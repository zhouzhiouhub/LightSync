# 快速检查插件 DLL
Write-Host "检查插件输出..." -ForegroundColor Cyan
Get-ChildItem out\LightSync\debug\plugins\*.dll | ForEach-Object {
    Write-Host "  ✓ $($_.Name)" -ForegroundColor Green
}

