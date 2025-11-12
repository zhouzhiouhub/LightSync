# 验证 Visual Map 插件构建配置
Write-Host "=== 检查 Visual Map 插件构建配置 ===" -ForegroundColor Cyan

# 1. 检查项目文件
Write-Host "`n[1] 检查项目文件..." -ForegroundColor Yellow
$proFile = "OpenRGBVisualMapPlugin\OpenRGBVisualMapPlugin.pro"
if (Test-Path $proFile) {
    Write-Host "  ✓ 找到项目文件: $proFile" -ForegroundColor Green
    
    # 检查 TARGET
    $target = Select-String -Path $proFile -Pattern "^TARGET\s*=" 
    if ($target) {
        Write-Host "  ✓ TARGET 已定义: $($target.Line.Trim())" -ForegroundColor Green
    } else {
        Write-Host "  ✗ 未找到 TARGET 定义" -ForegroundColor Red
    }
    
    # 检查 DESTDIR
    $destdir = Select-String -Path $proFile -Pattern "DESTDIR\s*=" 
    if ($destdir) {
        Write-Host "  ✓ DESTDIR 已定义: $($destdir.Line.Trim())" -ForegroundColor Green
    } else {
        Write-Host "  ✗ 未找到 DESTDIR 定义" -ForegroundColor Red
    }
} else {
    Write-Host "  ✗ 未找到项目文件" -ForegroundColor Red
}

# 2. 检查源文件
Write-Host "`n[2] 检查源文件..." -ForegroundColor Yellow
$cppFiles = Get-ChildItem "OpenRGBVisualMapPlugin" -Recurse -Filter "*.cpp" | Measure-Object
$hFiles = Get-ChildItem "OpenRGBVisualMapPlugin" -Recurse -Filter "*.h" | Measure-Object
Write-Host "  ✓ 找到 $($cppFiles.Count) 个 .cpp 文件" -ForegroundColor Green
Write-Host "  ✓ 找到 $($hFiles.Count) 个 .h 文件" -ForegroundColor Green

# 3. 检查主项目配置
Write-Host "`n[3] 检查主项目 LightSync.pro..." -ForegroundColor Yellow
$mainPro = "LightSync.pro"
if (Test-Path $mainPro) {
    $subdirs = Select-String -Path $mainPro -Pattern "SUBDIRS"
    Write-Host "  ✓ SUBDIRS 配置:" -ForegroundColor Green
    $subdirs | ForEach-Object { Write-Host "    $($_.Line.Trim())" }
    
    $depends = Select-String -Path $mainPro -Pattern "\.depends"
    Write-Host "  ✓ 依赖关系:" -ForegroundColor Green
    $depends | ForEach-Object { Write-Host "    $($_.Line.Trim())" }
}

# 4. 检查输出目录
Write-Host "`n[4] 检查输出目录..." -ForegroundColor Yellow
$debugPlugins = "out\LightSync\debug\plugins"
$releasePlugins = "out\LightSync\release\plugins"

if (Test-Path $debugPlugins) {
    $debugDlls = Get-ChildItem $debugPlugins -Filter "*.dll"
    Write-Host "  ✓ Debug 插件目录存在" -ForegroundColor Green
    Write-Host "    找到 $($debugDlls.Count) 个 DLL:" -ForegroundColor Cyan
    $debugDlls | ForEach-Object { Write-Host "      - $($_.Name)" }
} else {
    Write-Host "  ⚠ Debug 插件目录不存在" -ForegroundColor Yellow
}

if (Test-Path $releasePlugins) {
    $releaseDlls = Get-ChildItem $releasePlugins -Filter "*.dll"
    Write-Host "  ✓ Release 插件目录存在" -ForegroundColor Green
    Write-Host "    找到 $($releaseDlls.Count) 个 DLL:" -ForegroundColor Cyan
    $releaseDlls | ForEach-Object { Write-Host "      - $($_.Name)" }
} else {
    Write-Host "  ⚠ Release 插件目录不存在" -ForegroundColor Yellow
}

# 5. 检查中间构建目录
Write-Host "`n[5] 检查中间构建目录..." -ForegroundColor Yellow
$buildDir = "build\LightSync\visualmap"
if (Test-Path $buildDir) {
    Write-Host "  ✓ 找到中间构建目录: $buildDir" -ForegroundColor Green
    $objFiles = Get-ChildItem $buildDir -Recurse -Filter "*.obj" -ErrorAction SilentlyContinue
    if ($objFiles) {
        Write-Host "    找到 $($objFiles.Count) 个 .obj 文件" -ForegroundColor Cyan
    } else {
        Write-Host "    ⚠ 未找到 .obj 文件（可能未构建）" -ForegroundColor Yellow
    }
} else {
    Write-Host "  ⚠ 未找到中间构建目录（项目未构建）" -ForegroundColor Yellow
}

Write-Host "`n=== 诊断完成 ===" -ForegroundColor Cyan
Write-Host "`n建议操作:" -ForegroundColor Magenta
Write-Host "1. 在 Qt Creator 中右键点击 'LightSync' 项目" -ForegroundColor White
Write-Host "2. 选择 'Run qmake' (运行 qmake)" -ForegroundColor White
Write-Host "3. 选择 'Rebuild' (重新构建)" -ForegroundColor White
Write-Host "4. 检查 '编译输出' 窗口中是否有 Visual Map 相关的构建消息" -ForegroundColor White

