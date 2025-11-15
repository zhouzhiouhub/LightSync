# ✅ 已完成 - Debug 和 Release 版本分离配置

## 🎊 问题解决

感谢您指出这个问题！现在 Debug 和 Release 版本已经分离到不同的目录，不会相互覆盖。

---

## 📁 当前目录结构

```
out/LightSync/
├── debug/                          # Debug 版本 🐛
│   ├── OpenRGB.exe                 # 15.5 MB（含调试信息）
│   ├── OpenRGB.pdb                 # 82 MB（调试符号）
│   ├── hidapi.dll
│   ├── libusb-1.0.dll
│   ├── PawnIOLib.dll
│   ├── SmbusI801.bin
│   ├── SmbusPIIX4.bin
│   ├── SmbusNCT6793.bin
│   ├── LpcIO.bin
│   └── plugins/
│       ├── OpenRGBEffectsPlugin.dll    # 5.3 MB
│       └── OpenRGBEffectsPlugin.pdb    # 32 MB
│
└── release/                        # Release 版本 🚀
    ├── OpenRGB.exe                 # (将在构建时生成)
    ├── *.dll
    └── plugins/
        └── OpenRGBEffectsPlugin.dll
```

---

## ✅ 已完成的修改

### 1. 配置文件更新

#### OpenRGB/OpenRGB.pro
```qmake
win32:CONFIG(debug, debug|release) {
    BIN_SUBDIR = debug
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR  # 输出到 debug/
}

win32:CONFIG(release, debug|release) {
    BIN_SUBDIR = release
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR  # 输出到 release/
}
```

#### OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.pro
```qmake
win32:CONFIG(debug, debug|release) {
    BIN_SUBDIR = debug
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}

win32:CONFIG(release, debug|release) {
    BIN_SUBDIR = release
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}
```

### 2. 文档更新

- ✅ `README.md` - 更新目录结构说明
- ✅ `clean-build.ps1` - 更新清理脚本支持两个版本
- ✅ `docs/Version-Separation.md` - 新增版本分离说明文档

### 3. 文件迁移

- ✅ 现有 Debug 版本文件已移动到 `out/LightSync/debug/`

---

## 🚀 使用方式

### Debug 版本（开发调试）

```bash
# 运行 Debug 版本
cd out\LightSync\debug
.\OpenRGB.exe

# 特点：
# - 包含调试信息（.pdb 文件）
# - 文件较大
# - 可以使用调试器
# - 性能较慢
```

### Release 版本（发布使用）

```bash
# 构建 Release 版本
# 1. 在 Qt Creator 中切换到 Release 配置
# 2. 构建项目

# 运行 Release 版本
cd out\LightSync\release
.\OpenRGB.exe

# 特点：
# - 优化后的代码
# - 文件较小
# - 运行速度快
# - 用于正式发布
```

---

## 📊 版本对比

| 项目 | Debug 版本 | Release 版本 |
|-----|-----------|-------------|
| **输出目录** | `out/LightSync/debug/` | `out/LightSync/release/` |
| **主程序大小** | ~15.5 MB | 更小（优化后） |
| **包含 .pdb** | ✅ 是（82 MB） | ❌ 否 |
| **调试支持** | ✅ 完整 | ❌ 无 |
| **运行速度** | 较慢 | ⚡ 快 |
| **代码优化** | 无 | 全优化 |
| **适用场景** | 开发、调试、定位问题 | 发布、生产环境 |

---

## 💡 最佳实践

### 开发阶段
```bash
# 使用 Debug 版本
cd out\LightSync\debug
.\OpenRGB.exe

# 优势：
# - 便于调试
# - 错误信息详细
# - 可以断点调试
```

### 测试阶段
```bash
# 使用 Release 版本测试性能
cd out\LightSync\release
.\OpenRGB.exe

# 优势：
# - 测试真实性能
# - 验证用户体验
# - 发现发布版本特有问题
```

### 发布阶段
```bash
# 只分发 Release 版本
xcopy out\LightSync\release dist\LightSync /E /I /Y

# 优势：
# - 体积小
# - 性能好
# - 专业
```

---

## 🎯 下次构建时

### 构建 Debug 版本
1. 在 Qt Creator 选择 "Debug" 配置
2. 点击构建
3. 输出到 `out/LightSync/debug/`

### 构建 Release 版本
1. 在 Qt Creator 选择 "Release" 配置
2. 点击构建
3. 输出到 `out/LightSync/release/`

**两个版本可以同时存在，互不干扰！** ✨

---

## 🧹 清理

```powershell
# 清理所有构建文件（包括两个版本）
.\clean-build.ps1

# 或手动清理
Remove-Item -Path "out\LightSync\debug" -Recurse -Force
Remove-Item -Path "out\LightSync\release" -Recurse -Force
```

---

## 📚 相关文档

- [README.md](../README.md) - 项目主文档
- [Version-Separation.md](Version-Separation.md) - 版本分离详细说明
- [Build-Configuration.md](Build-Configuration.md) - 构建配置说明

---

<div align="center">

## ✅ 配置完成

**现在 Debug 和 Release 版本会输出到不同目录**

`out/LightSync/debug/` 🐛 开发调试用  
`out/LightSync/release/` 🚀 发布用

**两个版本互不干扰，可以同时存在！**

</div>

---

## 📅 更新时间

2025-11-11 20:53 - 完成 Debug/Release 版本分离配置

