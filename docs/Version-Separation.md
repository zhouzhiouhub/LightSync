# ✅ 配置更新 - 支持 Debug 和 Release 版本分离

## 🎯 更新内容

已修改构建配置，现在 Debug 和 Release 版本会输出到不同的目录，互不干扰。

---

## 📁 新的目录结构

```
out/
└── LightSync/
    ├── debug/                          # Debug 版本 🐛
    │   ├── OpenRGB.exe                 # Debug 主程序（含调试信息）
    │   ├── OpenRGB.pdb                 # 调试符号文件
    │   ├── *.dll                       # 依赖库
    │   ├── *.qm                        # 翻译文件
    │   └── plugins/
    │       ├── OpenRGBEffectsPlugin.dll
    │       └── OpenRGBEffectsPlugin.pdb
    │
    └── release/                        # Release 版本 🚀
        ├── OpenRGB.exe                 # Release 主程序（优化版本）
        ├── *.dll                       # 依赖库
        ├── *.qm                        # 翻译文件
        └── plugins/
            └── OpenRGBEffectsPlugin.dll
```

---

## 🔄 配置变更

### OpenRGB/OpenRGB.pro

```qmake
# Debug 版本输出到 out/LightSync/debug/
win32:CONFIG(debug, debug|release) {
    BIN_SUBDIR = debug
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR
}

# Release 版本输出到 out/LightSync/release/
win32:CONFIG(release, debug|release) {
    BIN_SUBDIR = release
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR
}
```

### OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.pro

```qmake
# Debug 插件输出到 out/LightSync/debug/plugins/
win32:CONFIG(debug, debug|release) {
    BIN_SUBDIR = debug
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}

# Release 插件输出到 out/LightSync/release/plugins/
win32:CONFIG(release, debug|release) {
    BIN_SUBDIR = release
    BUILD_BASE_DIR = $$PWD/../build/LightSync
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}
```

---

## 🚀 使用方法

### 构建 Debug 版本

在 Qt Creator 中选择 "Debug" 配置，然后构建：

```bash
# 输出到
out/LightSync/debug/OpenRGB.exe
```

### 构建 Release 版本

在 Qt Creator 中选择 "Release" 配置，然后构建：

```bash
# 输出到
out/LightSync/release/OpenRGB.exe
```

### 运行程序

**Debug 版本（用于开发调试）：**
```bash
cd out\LightSync\debug
.\OpenRGB.exe
```

**Release 版本（用于发布）：**
```bash
cd out\LightSync\release
.\OpenRGB.exe
```

---

## 📊 版本对比

| 特性 | Debug 版本 | Release 版本 |
|-----|-----------|-------------|
| **文件大小** | 较大（含调试信息） | 较小（优化后） |
| **运行速度** | 较慢 | 快速 |
| **调试支持** | ✅ 完整支持 | ❌ 无调试信息 |
| **优化级别** | 无优化 | 全优化 |
| **适用场景** | 开发、调试 | 发布、生产环境 |
| **包含 .pdb** | ✅ 是 | ❌ 否（或可选） |

---

## ✨ 优势

1. **版本隔离** - Debug 和 Release 互不干扰，可同时存在
2. **清晰标识** - 目录名称明确标识版本类型
3. **便于测试** - 可以对比两个版本的性能和行为
4. **简化部署** - Release 版本可直接用于分发
5. **调试便利** - Debug 版本包含完整调试信息

---

## 📦 部署建议

### 开发阶段
使用 Debug 版本进行开发和调试：
```
out/LightSync/debug/
```

### 发布阶段
只分发 Release 版本：
```bash
# 复制 release 文件夹用于分发
xcopy out\LightSync\release dist\LightSync /E /I /Y
```

---

## 🧹 清理

使用更新后的清理脚本：

```powershell
.\clean-build.ps1
```

这会清理两个版本的输出目录。

---

## 🎓 最佳实践

1. **开发时使用 Debug** - 便于调试和定位问题
2. **测试使用 Release** - 验证实际性能和体验
3. **发布使用 Release** - 给用户提供优化后的版本
4. **版本控制** - 不提交 `out/` 目录到 Git

---

## 📅 更新日期

**2025-11-11**: 添加 Debug/Release 版本分离支持

---

<div align="center">

**🎊 现在 Debug 和 Release 版本会输出到不同目录，互不干扰！**

</div>

