# 快速使用指南

## 静态插件集成 - LightSync

### 🎯 功能概述

LightSync 现在将 OpenRGB Effects 和 Visual Map 插件直接集成到主程序中，程序启动时自动加载，无需额外配置。

### 🔧 编译步骤

#### Windows (使用 Qt Creator 或命令行)

```bash
# 1. 打开 Qt 命令提示符 (Qt Command Prompt)
# 2. 进入项目目录
cd D:\Commonly_used_files\zhou\LightSync

# 3. 生成 Makefile
qmake LightSync.pro

# 4. 编译
nmake        # 使用 MSVC
# 或
mingw32-make # 使用 MinGW
```

#### Linux

```bash
# 1. 进入项目目录
cd ~/LightSync

# 2. 清理旧的构建（可选）
make clean

# 3. 生成 Makefile
qmake LightSync.pro

# 4. 编译
make -j$(nproc)
```

#### macOS

```bash
# 1. 进入项目目录
cd ~/LightSync

# 2. 生成 Makefile
qmake LightSync.pro

# 3. 编译
make -j$(sysctl -n hw.ncpu)
```

### ✅ 验证集成

启动编译后的 OpenRGB，你应该看到：

1. **Effects 标签页** - 位于顶层标签栏（静态集成）
2. **Visual Map 标签页** - 位于顶层标签栏（静态集成）
3. 这两个插件在"插件"设置页中显示为"系统插件"
4. **仍然支持外部动态插件** - 可以在用户配置目录的 `plugins` 文件夹中放置额外插件

### 🔌 外部插件支持

虽然 Effects 和 Visual Map 已经内置，但程序仍然支持从以下位置加载额外的动态插件：

- **Windows**: `%APPDATA%\OpenRGB\plugins\`
- **Linux**: `~/.config/OpenRGB/plugins/`
- **macOS**: `~/Library/Application Support/OpenRGB/plugins/`

将第三方插件 DLL/SO 文件放入上述目录，程序会在设备检测完成后自动加载。

详细说明请参考 [`DYNAMIC_PLUGINS_SUPPORT.md`](DYNAMIC_PLUGINS_SUPPORT.md)。

### 📁 输出位置

- **Windows**: `out/LightSync/release/OpenRGB.exe` 或 `debug/OpenRGB.exe`
- **Linux**: 根据配置，通常在构建目录中
- **macOS**: 根据配置，通常在构建目录中

### 🔍 对比原版

#### 原版 OpenRGB (动态插件)
```
OpenRGB/
├── OpenRGB.exe
└── plugins/
    ├── OpenRGBEffectsPlugin.dll
    └── OpenRGBVisualMapPlugin.dll
```

#### LightSync (静态集成)
```
LightSync/
└── OpenRGB.exe  ← 包含所有插件
```

### 📊 技术细节

| 特性 | 原版 | LightSync |
|------|------|-----------|
| 插件加载方式 | 动态加载 DLL | 静态链接 + 动态加载 |
| 启动时间 | 慢（需要加载DLL） | 快（内置插件直接可用） |
| 文件数量 | 多个文件 | 单一可执行文件 + 可选外部插件 |
| 部署复杂度 | 需要完整目录 | 仅需可执行文件（基础功能） |
| 插件更新 | 独立更新插件 | 内置插件需重编译，外部插件可独立更新 |
| 扩展性 | 支持外部插件 | 同样支持外部插件 |

### 🐛 故障排除

#### 编译错误：找不到插件头文件
- 确保 `OpenRGBEffectsPlugin` 和 `OpenRGBVisualMapPlugin` 目录在项目根目录下
- 检查 `plugins_static.pri` 中的路径是否正确

#### 链接错误：未定义的引用
- 检查平台特定的库是否安装（如 Windows 的 OpenGL32，Linux 的 OpenAL、PipeWire）
- 确保 Qt 版本匹配（建议 Qt 5.15+ 或 Qt 6.2+）

#### 运行时错误：插件未加载
- 检查日志输出，应该看到 `[StaticPluginManager] Loading static plugins`
- 确保 `StaticPluginManager::LoadStaticPlugins()` 被调用

### 💡 开发建议

#### 添加新的静态插件

1. 编辑 `OpenRGB/StaticPluginManager.cpp`:
```cpp
void StaticPluginManager::LoadStaticPlugins()
{
    // 现有插件...
    
    // 添加新插件
    MyNewPlugin* new_plugin = new MyNewPlugin();
    RegisterPlugin(new_plugin);
}
```

2. 编辑 `OpenRGB/plugins_static.pri`:
```qmake
INCLUDEPATH += ../MyNewPlugin
HEADERS += ../MyNewPlugin/MyNewPlugin.h
SOURCES += ../MyNewPlugin/MyNewPlugin.cpp
```

#### 调试静态插件

在 `StaticPluginManager::RegisterPlugin()` 中设置断点，查看插件注册过程。

### 📝 更多信息

- **静态插件技术详情**: 查看 [`STATIC_PLUGINS_INTEGRATION.md`](STATIC_PLUGINS_INTEGRATION.md)
- **动态插件支持说明**: 查看 [`DYNAMIC_PLUGINS_SUPPORT.md`](DYNAMIC_PLUGINS_SUPPORT.md)
- **快速指南**: 本文档

### 🤝 贡献

如果你发现问题或有改进建议，请提交 Issue 或 Pull Request。

---

**注意**: 这个集成方案专为 LightSync 项目设计，遵循 SOLID、DRY、SRP 原则，确保高内聚、低耦合。

