### 统一编译 OpenRGB 与 Effects 插件

- 使用根目录 `LightSync.pro`（qmake SUBDIRS）一次性编译 OpenRGB 与插件。
- Windows 下编译产物：
  - `OpenRGB/<config>/OpenRGB.exe`
  - 插件自动复制到：`OpenRGB/<config>/plugins/`

#### 步骤
1. 打开 Qt Creator 或使用命令行进入仓库根目录。
2. 选择并加载 `LightSync.pro`。
3. 选择 Debug/Release 构建配置并构建。
4. 运行 `OpenRGB/<config>/OpenRGB.exe`，插件会被自动发现并加载。

备注：Linux/macOS 仍可按各自 `.pro` 的安装路径规则运行（系统插件目录见 `OPENRGB_SYSTEM_PLUGIN_DIRECTORY` 定义）。
