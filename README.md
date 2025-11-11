# LightSync - 统一构建版本

## 简介

LightSync 是 OpenRGB 和 OpenRGBEffectsPlugin 的统一构建版本，将两个项目合并到一个输出目录中，简化部署和使用。

## 快速开始

### 构建项目

```bash
# 1. 使用 Qt Creator 打开项目
# 打开 LightSync.pro

# 2. 选择构建配置（Debug 或 Release）

# 3. 构建项目
# 所有输出文件将自动生成到 out/LightSync/
```

### 运行程序

直接运行生成的可执行文件：

**Debug 版本：**
```bash
cd out/LightSync/debug
OpenRGB.exe
```

**Release 版本：**
```bash
cd out/LightSync/release
OpenRGB.exe
```

## 目录结构

```
LightSync/
├── OpenRGB/                 # OpenRGB 主程序源码
├── OpenRGBEffectsPlugin/    # 特效插件源码
├── out/                     # 构建输出目录
│   └── LightSync/           # 统一输出文件夹 ⭐
│       ├── debug/           # Debug 版本 🐛
│       │   ├── OpenRGB.exe
│       │   ├── *.dll
│       │   ├── *.qm         # 翻译文件
│       │   └── plugins/
│       │       └── OpenRGBEffectsPlugin.dll
│       └── release/         # Release 版本 🚀
│           ├── OpenRGB.exe
│           ├── *.dll
│           ├── *.qm
│           └── plugins/
│               └── OpenRGBEffectsPlugin.dll
├── docs/                    # 文档
│   ├── Build-Output-Changes.md  # 构建配置说明
│   └── Unified-Build.md         # 统一构建指南
└── LightSync.pro            # 主项目文件
```

## 主要特性

- ✅ **统一输出** - 所有文件集中在 `out/LightSync/` 目录
- ✅ **版本分离** - Debug 和 Release 版本独立存放
- ✅ **自动插件加载** - 插件自动从 `plugins/` 文件夹加载
- ✅ **简化部署** - 直接复制对应版本文件夹即可分发
- ✅ **多语言支持** - 包含 20+ 种语言的翻译文件

## 配置变更

相比原始 OpenRGB 项目，主要变更：

1. **输出目录统一** - Debug 和 Release 分别输出到 `LightSync/debug/` 和 `LightSync/release/`
2. **插件集成** - OpenRGBEffectsPlugin 自动构建到对应版本的 `plugins/` 子文件夹
3. **依赖管理** - 所有 DLL 依赖自动复制到输出目录
4. **版本隔离** - Debug 和 Release 版本互不干扰

详细信息请参考 [构建输出配置说明](docs/Build-Output-Changes.md)

## 系统要求

- Windows 10 或更高版本
- Qt 5.15.2 或更高版本
- MSVC 2019 编译器（或兼容版本）

## 开发指南

### 设计原则

本项目遵循以下设计原则：
- **SOLID 原则** - 单一职责、开闭原则
- **DRY 原则** - 不重复代码
- **高内聚、低耦合** - UI 与核心逻辑分离

### 添加新功能

1. 在相应的源码目录中添加新文件
2. 更新对应的 `.pro` 文件
3. 重新构建项目

## 许可证

- OpenRGB: GPLv2
- OpenRGBEffectsPlugin: GPLv2

## 贡献

欢迎提交 Issue 和 Pull Request！

## 链接

- [OpenRGB 官方仓库](https://gitlab.com/CalcProgrammer1/OpenRGB)
- [OpenRGBEffectsPlugin 官方仓库](https://gitlab.com/OpenRGBDevelopers/OpenRGBEffectsPlugin)

