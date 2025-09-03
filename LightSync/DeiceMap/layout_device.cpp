
// 画布坐标系：左上(0,0)，单位px
struct Vec2 { float x, y; };
struct Rect { float x,y,w,h; };

enum OrderMode { LeftToRight, CenterToOutside, OutsideToCenter };
enum Orientation { Forward, Reverse };   // LED顺序方向
enum EffectType { Solid, Gradient, Rainbow, Breathe, Comet, Ambilight, AudioReactive };

struct EffectPreset {
    EffectType type;
    float speed;             // 通用速度
    float hueShift;          // 色调偏移(统一效果常用)
    float saturation;        // 0..2
    float brightness;        // 0..1
    // Ambilight专用
    int monitorId;
    Rect captureZone;        // 归一化或像素均可
    float edgeDistance;
    // 其他效果的参数...
};

struct DeviceNode {
    string id;               // 设备唯一ID
    string name;
    int    leds;             // LED数量
    vector<Vec2> ledMap;     // 可选：归一化0..1坐标；为空用均匀分布
    Rect   bounds;           // 在布局器上的矩形(位置+尺寸)
    float  rotationDeg;      // 0/90/180/270 or 任意
    Orientation orientation; // 正向或反向
    // 设备效果覆盖(可选)
    bool           hasLocalEffect = false;
    EffectPreset   localEffect;
    // 运行期缓存
    vector<uint32_t> frame;  // 本帧颜色缓冲(ARGB)
};

struct LayoutState {
    vector<DeviceNode> devices;
    // 统一效果(当没有localEffect或选择“统一控制”时生效)
    bool useUnifiedEffect = true;
    EffectPreset unifiedEffect;
    OrderMode    orderMode = LeftToRight;
    // 排序计算缓存
    vector<int>  orderedDeviceIdx; // 排序后的设备索引
    // 画布参数
    Rect canvas; // 画布大小
};