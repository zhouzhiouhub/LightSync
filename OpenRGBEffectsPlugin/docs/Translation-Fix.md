# OpenRGB Effects Plugin 翻译修复文档

## 问题分析

### 根本原因
1. **翻译文件未正确打包**：`.qm` 文件虽然被编译并添加到 `resources.qrc`，但资源路径不匹配
2. **资源路径错误**：
   - 代码尝试加载：`:/i18n/OpenRGB_EffectsEngine_zh_CN.qm`
   - 实际资源路径：`:/i18n/i18n/OpenRGB_EffectsEngine_zh_CN.qm`（多了一层 `i18n/`）
3. **初始化逻辑问题**：`current_i18n_file` 初始值为 "default"，首次启动跳过加载

## 已完成的修复

### 1. 修复了 XML 错误（已合并）
**文件**: `OpenRGBEffectsPlugin/i18n/OpenRGB_EffectsEngine_zh_CN.ts`
- 移除了第 66 行多余的 `</message>` 标签

### 2. 修复了初始化逻辑（已合并）
**文件**: `OpenRGBEffectsPlugin/OpenRGBEffectTab.h`
```cpp
// 从 "default" 改为 ""，确保首次启动加载翻译
std::string current_i18n_file = "";
```

### 3. 增强了翻译加载（已合并）
**文件**: `OpenRGBEffectsPlugin/OpenRGBEffectTab.cpp`
- 添加了调试输出（`printf`）
- 增加了 fallback 路径（`:/translations/`, 应用目录）
- 添加了加载失败警告

### 4. 修复了资源路径（待重新编译）
**文件**: `OpenRGBEffectsPlugin/resources.qrc`

**修复前**（错误）：
```xml
<qresource prefix="/i18n">
    <file>i18n/OpenRGB_EffectsEngine_zh_CN.qm</file>
</qresource>
```
→ 实际路径：`:/i18n/i18n/OpenRGB_EffectsEngine_zh_CN.qm` ❌

**修复后**（正确）：
```xml
<qresource prefix="/i18n">
    <file alias="OpenRGB_EffectsEngine_zh_CN.qm">i18n/OpenRGB_EffectsEngine_zh_CN.qm</file>
</qresource>
```
→ 实际路径：`:/i18n/OpenRGB_EffectsEngine_zh_CN.qm` ✅

### 5. 创建了自动化脚本
**文件**: `OpenRGBEffectsPlugin/compile-translations.ps1`
- 自动编译 `.ts` → `.qm`
- 自动更新 `resources.qrc`（使用 `alias` 属性）
- 支持多版本 Qt 路径自动查找

## 如何使用

### 方法 1：自动脚本（推荐）
```powershell
cd OpenRGBEffectsPlugin
powershell -ExecutionPolicy Bypass -File .\compile-translations.ps1
```

### 方法 2：手动编译
```bash
cd OpenRGBEffectsPlugin/i18n
lrelease OpenRGB_EffectsEngine_zh_CN.ts -qm OpenRGB_EffectsEngine_zh_CN.qm
```

## 验证步骤

### 编译后验证
1. 重新编译插件（完整 rebuild）
2. 启动 OpenRGB
3. 在中文系统或设置中选择"简体中文"
4. 打开 **Effects** 标签页
5. 查看菜单是否显示中文：
   - ✅ Profiles → 简介
   - ✅ Advanced → 高阶灯效
   - ✅ Audio → 音频
   - ✅ Rainbow → 彩虹
   - ✅ Settings → 设置
   - ✅ About → 关于
   - ✅ Rainbow Wave → 彩虹波浪

### 调试输出
启动时控制台应显示：
```
[OpenRGBEffectsPlugin] Language setting from UI: default
[OpenRGBEffectsPlugin] Current i18n file: , New file: default, System locale: zh_CN
[OpenRGBEffectsPlugin] Translation loaded successfully, installing translator
```

如果加载失败会显示：
```
[OpenRGBEffectsPlugin] WARNING: Failed to load translation file for locale zh_CN
```

## 故障排查

### 问题：重新编译后仍显示英文

#### 检查 1：确认 .qm 文件存在
```powershell
ls OpenRGBEffectsPlugin\i18n\*.qm
```
应该看到 `OpenRGB_EffectsEngine_zh_CN.qm`（约 35KB）

#### 检查 2：确认资源文件正确
```powershell
Select-String -Path "build\LightSync\_i_debug\r\qrc_resources.cpp" -Pattern "OpenRGB_EffectsEngine_zh_CN"
```
应该看到 `:/i18n/OpenRGB_EffectsEngine_zh_CN.qm`（只有一个 `i18n/`）

#### 检查 3：查看控制台输出
运行 OpenRGB，查看 `[OpenRGBEffectsPlugin]` 开头的调试信息

#### 检查 4：强制 Clean 后重新编译
有时需要完全清理构建缓存：
```
右键项目 → Clean → Rebuild All
```

## 技术细节

### Qt 资源系统路径规则
```
<qresource prefix="/i18n">
    <file>path/to/file.qm</file>
</qresource>
```
→ 资源路径：`:/i18n/path/to/file.qm`

```
<qresource prefix="/i18n">
    <file alias="file.qm">path/to/file.qm</file>
</qresource>
```
→ 资源路径：`:/i18n/file.qm` ✅（推荐）

### 翻译加载顺序
1. 从主应用获取语言设置（`ComboBoxLanguage`）
2. 替换 `OpenRGB` → `OpenRGB_EffectsEngine`
3. 尝试加载（多个 fallback 路径）：
   - `:/i18n/OpenRGB_EffectsEngine_zh_CN.qm`
   - `:/translations/OpenRGB_EffectsEngine_zh_CN.qm`
   - `<app_dir>/i18n/OpenRGB_EffectsEngine_zh_CN.qm`
4. 安装 translator 并重建菜单

## 相关文件
- `OpenRGBEffectTab.cpp` - 翻译加载逻辑
- `OpenRGBEffectTab.h` - translator 成员变量
- `EffectList.cpp` - 菜单构建（使用 `QCoreApplication::translate`）
- `RGBEffect.h` - 分类定义（使用 `QT_TRANSLATE_NOOP`）
- `resources.qrc` - Qt 资源文件
- `compile-translations.ps1` - 自动化编译脚本

## 维护说明

### 添加新翻译时
1. 编辑 `.ts` 文件
2. 运行 `compile-translations.ps1`
3. 重新编译插件

### 更新现有翻译
1. 运行 `lupdate OpenRGBEffectsPlugin.pro`（生成新字符串）
2. 在 Qt Linguist 中编辑翻译
3. 运行 `compile-translations.ps1`
4. 重新编译

---
**最后更新**: 2025-11-17  
**状态**: 待验证（需要重新编译插件确认修复生效）

