# 动态插件支持说明

## 概述

虽然 OpenRGBEffectsPlugin 和 OpenRGBVisualMapPlugin 已经静态集成到主程序中，但是 **仍然保留了对外部动态插件的支持**。用户可以在配置目录的 `plugins` 文件夹中放置额外的插件 DLL/SO 文件，程序会自动加载它们。

## 插件目录位置

### Windows
```
%APPDATA%\OpenRGB\plugins\
```
通常展开为：
```
C:\Users\[用户名]\AppData\Roaming\OpenRGB\plugins\
```

### Linux
```
~/.config/OpenRGB/plugins/
```

### macOS
```
~/Library/Application Support/OpenRGB/plugins/
```

## 插件类型

LightSync 现在支持两种类型的插件：

### 1. 静态插件（内置插件）
- **OpenRGBEffectsPlugin** - 效果插件
- **OpenRGBVisualMapPlugin** - 可视化地图插件
- 这些插件编译到主程序中，启动时自动加载
- 无法禁用
- 在插件列表中标记为"系统插件"

### 2. 动态插件（外部插件）
- 从用户配置目录加载的 DLL/SO 文件
- 在设备检测完成后自动扫描和加载
- 可以通过插件管理页面启用/禁用
- 支持热加载（无需重启程序）

## 加载时序

```
程序启动
    ↓
初始化 StaticPluginManager
    ↓
加载静态插件（Effects、Visual Map）
    ↓
初始化 PluginManager
    ↓
设备检测开始
    ↓
设备检测完成
    ↓
扫描用户配置目录 plugins 文件夹
    ↓
加载动态插件（如果有）
    ↓
程序运行
```

## 创建第三方动态插件

### 1. 插件开发要求

- 必须继承 `OpenRGBPluginInterface`
- 必须包含 Qt 插件系统宏：
  ```cpp
  Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
  Q_INTERFACES(OpenRGBPluginInterface)
  ```
- 插件 API 版本必须匹配主程序

### 2. 插件项目配置示例

```qmake
TEMPLATE = lib
CONFIG += plugin
TARGET = MyCustomPlugin

QT += core gui widgets

# 包含 OpenRGB SDK
INCLUDEPATH += \
    /path/to/OpenRGB \
    /path/to/OpenRGB/RGBController

HEADERS += \
    /path/to/OpenRGB/OpenRGBPluginInterface.h \
    /path/to/OpenRGB/ResourceManagerInterface.h \
    MyCustomPlugin.h

SOURCES += \
    MyCustomPlugin.cpp
```

### 3. 插件头文件示例

```cpp
#ifndef MYCUSTOMPLUGIN_H
#define MYCUSTOMPLUGIN_H

#include "OpenRGBPluginInterface.h"
#include "ResourceManagerInterface.h"
#include <QObject>
#include <QString>
#include <QtPlugin>

class MyCustomPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    virtual OpenRGBPluginInfo   GetPluginInfo() override;
    virtual unsigned int        GetPluginAPIVersion() override;
    virtual void                Load(ResourceManagerInterface* resource_manager_ptr) override;
    virtual QWidget*            GetWidget() override;
    virtual QMenu*              GetTrayMenu() override;
    virtual void                Unload() override;

private:
    ResourceManagerInterface* rm;
};

#endif
```

### 4. 插件实现示例

```cpp
#include "MyCustomPlugin.h"

OpenRGBPluginInfo MyCustomPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    info.Name        = "My Custom Plugin";
    info.Description = "This is my custom plugin";
    info.Version     = "1.0.0";
    info.Commit      = "";
    info.URL         = "https://example.com";
    info.Label       = "My Plugin";
    info.Location    = OPENRGB_PLUGIN_LOCATION_TOP;
    return info;
}

unsigned int MyCustomPlugin::GetPluginAPIVersion()
{
    return OPENRGB_PLUGIN_API_VERSION;
}

void MyCustomPlugin::Load(ResourceManagerInterface* resource_manager_ptr)
{
    rm = resource_manager_ptr;
    // 初始化插件
}

QWidget* MyCustomPlugin::GetWidget()
{
    // 返回插件的 UI 界面
    return new QLabel("Hello from My Custom Plugin!");
}

QMenu* MyCustomPlugin::GetTrayMenu()
{
    // 返回系统托盘菜单项（可选）
    return nullptr;
}

void MyCustomPlugin::Unload()
{
    // 清理资源
}
```

## 安装第三方插件

### 方法 1：手动安装

1. 编译插件生成 DLL/SO 文件
2. 将文件复制到用户配置目录的 `plugins` 文件夹
   - Windows: `MyPlugin.dll`
   - Linux: `libMyPlugin.so`
   - macOS: `libMyPlugin.dylib`
3. 启动或重启 OpenRGB
4. 插件会在设备检测完成后自动加载

### 方法 2：通过插件管理页面

1. 打开 OpenRGB 主界面
2. 进入"设置" → "插件"
3. 点击"安装插件"
4. 选择插件 DLL/SO 文件
5. 插件会被自动复制到配置目录并加载

## 插件管理

### 启用/禁用插件

1. 打开插件管理页面
2. 找到要管理的插件
3. 点击"启用"或"禁用"按钮
4. 静态插件（标记为"系统插件"）无法禁用

### 卸载插件

1. 打开插件管理页面
2. 点击插件旁的"卸载"按钮
3. 插件文件会从配置目录删除
4. 需要重启程序才能完全卸载

## 插件冲突处理

### 名称冲突
如果动态插件与静态插件同名：
- 静态插件优先
- 动态插件会被标记为不兼容
- 日志中会显示警告信息

### API 版本不匹配
如果插件 API 版本与主程序不匹配：
- 插件会被标记为不兼容
- 不会加载该插件
- 插件列表中显示"不兼容"

## 调试动态插件

### 启用详细日志

在设置中启用日志控制台：
1. 打开"设置" → "日志管理器"
2. 启用"日志控制台"
3. 查看插件加载过程的详细信息

### 常见问题

#### 插件未出现在列表中
- 检查文件名是否正确（Windows: `.dll`, Linux: `.so`, macOS: `.dylib`）
- 检查文件是否在正确的目录中
- 查看日志中是否有错误信息

#### 插件加载失败
- 检查插件 API 版本是否匹配
- 确保插件依赖的库已安装
- 查看日志中的详细错误信息

#### 插件功能不正常
- 检查插件是否已启用
- 确保设备已正确检测
- 重启程序尝试

## 示例：开发简单的颜色循环插件

完整示例代码可以参考 OpenRGB 官方文档：
https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/OpenRGB-Plugin-SDK

## 性能考虑

- **静态插件**: 启动快，性能最优，无动态库加载开销
- **动态插件**: 灵活，可独立更新，但有少量加载开销

## 安全注意事项

⚠️ **警告**: 只从可信来源安装插件！插件有完整的程序访问权限，恶意插件可能：
- 窃取数据
- 损坏系统
- 导致程序崩溃

建议：
- 只安装开源且经过审核的插件
- 检查插件源代码（如果可能）
- 从官方渠道下载

## 技术支持

如果你在开发或使用动态插件时遇到问题：

1. 查看 [OpenRGB 官方文档](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home)
2. 检查 [插件开发指南](https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/OpenRGB-Plugin-SDK)
3. 在 OpenRGB Discord 或 Reddit 社区寻求帮助

---

**总结**: LightSync 完美结合了内置静态插件的便利性和外部动态插件的灵活性，为用户提供了最佳体验！

