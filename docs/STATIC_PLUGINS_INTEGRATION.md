# 静态插件集成说明

## 概述

本次修改将 OpenRGBEffectsPlugin 和 OpenRGBVisualMapPlugin 从动态加载的插件（DLL/SO）改为静态链接到主程序中。程序启动时会自动加载这些插件，无需从 plugins 目录加载额外的DLL文件。

## 架构变更

### 1. 原架构
- 插件编译为独立的 DLL/SO 文件
- 使用 Qt 的 QPluginLoader 动态加载
- 插件需要包含 Q_PLUGIN_METADATA 和 Q_INTERFACES 宏
- 插件目录扫描和加载由 PluginManager 处理

### 2. 新架构
- 插件源码直接编译到主程序可执行文件中
- 新增 StaticPluginManager 直接实例化插件对象
- 移除了 Qt 插件系统的宏定义
- 保留 PluginManager 用于可能的外部插件（可选）

## 修改的文件

### 核心文件

1. **OpenRGB/StaticPluginManager.h** (新建)
   - 静态插件管理器头文件
   - 负责直接实例化和管理内置插件

2. **OpenRGB/StaticPluginManager.cpp** (新建)
   - 实现静态插件的创建、注册、加载和卸载
   - 在构造时创建 OpenRGBEffectsPlugin 和 OpenRGBVisualMapPlugin 实例

3. **OpenRGB/plugins_static.pri** (新建)
   - QMake 配置文件，包含所有插件源文件
   - 定义插件相关的 INCLUDEPATH、HEADERS、SOURCES、FORMS
   - 处理平台特定的编译配置

4. **OpenRGBEffectsPlugin/effects_list.pri** (新建)
   - 包含所有效果实现的源文件列表
   - 由 plugins_static.pri 引用

### 修改的文件

5. **OpenRGB/OpenRGB.pro**
   - 添加了 `include(plugins_static.pri)` 来包含静态插件源码
   - 添加了 StaticPluginManager 的头文件和源文件

6. **LightSync.pro**
   - 移除了 OpenRGBEffectsPlugin 和 OpenRGBVisualMapPlugin 的独立编译
   - 现在只编译 OpenRGB 主程序（包含集成的插件）

7. **OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.h**
   - 注释掉了 Q_PLUGIN_METADATA 和 Q_INTERFACES 宏
   - 保留了 QObject 继承以支持 Qt 信号槽机制

8. **OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.h**
   - 注释掉了 Q_PLUGIN_METADATA 和 Q_INTERFACES 宏
   - 保留了 QObject 继承以支持 Qt 信号槽机制

9. **OpenRGB/qt/OpenRGBDialog/OpenRGBDialog.h**
   - 添加了 StaticPluginManager 头文件包含
   - 添加了 static_plugin_manager 成员变量

10. **OpenRGB/qt/OpenRGBDialog/OpenRGBDialog.cpp**
    - 在构造函数中创建 StaticPluginManager 并加载静态插件
    - 在析构函数中清理 StaticPluginManager
    - StaticPluginManager 在 PluginManager 之后初始化，确保回调正确设置

## 使用说明

### 编译

```bash
# 清理之前的构建
rm -rf build out

# 使用 qmake 生成 Makefile
qmake LightSync.pro

# 编译
make

# 或者在 Windows 上使用 nmake 或 mingw32-make
```

### 运行

程序启动后，Effects 和 Visual Map 插件会自动加载，无需任何额外配置。这两个插件的标签页会自动出现在主界面中。

### 插件管理页面

- 插件管理页面仍然存在
- 静态插件会显示在插件列表中
- 静态插件始终启用，无法通过UI禁用
- 如果需要，仍然可以从 plugins 目录加载动态插件

## 优势

1. **简化部署**: 不需要单独的插件 DLL 文件
2. **启动速度**: 避免了动态库加载的开销
3. **更好的集成**: 编译时链接，可以进行更多优化
4. **减少错误**: 避免了插件版本不匹配的问题
5. **更小的安装包**: 减少了文件数量和总体大小

## 注意事项

1. **构建依赖**: 插件的所有依赖必须在编译时可用（如 OpenAL、PipeWire 等）
2. **平台特定代码**: 不同平台的特定代码通过条件编译处理（win32、unix、macx）
3. **Qt 版本**: 某些功能（如 Qt5Compat）只在 Qt 6+ 中需要
4. **插件更新**: 要更新插件功能，需要重新编译整个程序

## 扩展性

如果将来需要添加更多静态插件：

1. 在 `StaticPluginManager::LoadStaticPlugins()` 中添加插件实例化代码
2. 在 `plugins_static.pri` 中添加插件的源文件
3. 确保插件头文件已经移除了 Qt 插件系统宏

## 回退到动态插件（如果需要）

如果需要回退到原来的动态插件架构：

1. 恢复插件头文件中的 Q_PLUGIN_METADATA 和 Q_INTERFACES 宏
2. 在 LightSync.pro 中重新添加插件子项目
3. 从 OpenRGB.pro 中移除 `include(plugins_static.pri)`
4. 在 OpenRGBDialog.cpp 中移除 StaticPluginManager 相关代码
5. 使用 PluginManager 的 ScanAndLoadPlugins() 方法加载插件

## 测试建议

1. 验证所有效果功能正常工作
2. 验证 Visual Map 插件功能正常
3. 检查插件设置的保存和加载
4. 测试托盘菜单中的插件项
5. 验证插件在不同设备配置下的行为
6. 检查内存泄漏（插件加载/卸载）

## 性能考虑

- **编译时间**: 首次编译时间会增加，因为需要编译所有插件源码
- **增量编译**: 修改插件代码后只重新编译修改的部分
- **可执行文件大小**: 可执行文件会变大（包含所有插件代码）
- **运行时性能**: 启动速度更快，运行时性能基本相同

## 许可证

所有修改遵循 OpenRGB 项目的 GPL-2.0-only 许可证。

