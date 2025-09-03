// ================== 运行期参数（来自 UI） ==================
struct UiParams {
    int   fps = 30;                 // 目标帧率
    int   monitorId = 0;            // 屏幕源编号
    float samplingRatio = 1.0f;     // 抽样比例(0.1~1.0)
    float smoothing = 0.5f;         // EMA 平滑系数 α (0~1)
    float edgeDistance = 0.06f;     // 内缩百分比 0~0.15（未启用自定义区域时生效）
    float hueOffsetDeg = 0.0f;      // 色调偏移 [-180,180]
    float colorTemperatureK = 6500; // 色温(开尔文)
    float saturationFactor = 1.0f;  // 饱和度倍率 [0,2]；1=原样

    bool  useCustomRegion = false;  // 是否启用自定义同屏区域
    // 像素/相对值任选其一，推荐像素；若是百分比可先换算为像素
    int   regionLeft   = 0;         // 自定义区域：左
    int   regionTop    = 0;         // 自定义区域：上
    int   regionWidth  = 0;         // 自定义区域：宽
    int   regionHeight = 0;         // 自定义区域：高

    enum Mode { ScreenAmbilight, Patterns } mode = ScreenAmbilight;
};

// ================== 设备配置（可多设备） ==================
struct DeviceCfg {
    std::string id;

    // 高级同屏：真实 LED 坐标(归一化 0..1，原点在左下)
    std::vector<Vec2> ledMap; // 非空则优先用精确坐标

    // 降级：四边均匀数量（任意边可为 0）
    int top=0, right=0, bottom=0, left=0;

    // 设备级校正（可选）
    float gamma = 2.2f;
    Vec3  whiteBalance = {1.0f, 1.0f, 1.0f}; // R,G,B 增益
};

// ================== 状态缓存（用于平滑） ==================
std::unordered_map<std::string, std::vector<ColorRGBA>> lastFrameColor; // device id -> 上一帧颜色

// ================== 主流程 ==================
void startAmbilight(const UiParams& ui, const std::vector<DeviceCfg>& devices) {

    if (ui.mode != UiParams::ScreenAmbilight) return; // 其他模式另行处理

    ScreenCapturer cap;                   // TODO: 绑定你现有的抓屏实现
    cap.bindMonitor(ui.monitorId);
    cap.setFps(ui.fps);

    while (running) {
        // 1) 抓屏：一次抓取，复用给所有设备
        Bitmap frame = cap.capture();     // 格式 RGBA/BGRA，记清楚
        if (frame.empty()) { sleepUntilNextFrame(ui.fps); continue; }

        // 2) 降采样：按 samplingRatio 缩放以提速
        Bitmap sampled = downsample(frame, ui.samplingRatio); // TODO: 双线性缩放

        // 3) 计算采样区域
        Rect inner;
        if (ui.useCustomRegion) {
            // 自定义矩形，优先级最高
            inner = Rect(ui.regionLeft, ui.regionTop, ui.regionWidth, ui.regionHeight);
            inner = clampToScreen(inner, sampled.size()); // TODO: 与图像边界相交
            if (inner.width <= 0 || inner.height <= 0) {
                // 无效区域直接跳过本帧
                sleepUntilNextFrame(ui.fps);
                continue;
            }
        } else {
            // 默认：按 edgeDistance 从四边内缩
            inner = shrinkToInnerRect(sampled.rect(), ui.edgeDistance); // TODO
        }

        // 4) 针对每台设备映射 + 后处理 + 平滑 + 下发
        for (const auto& dev : devices) {

            // 4.1 映射：得到与 LED 数一致的一维颜色数组
            std::vector<ColorRGBA> colors;
            if (!dev.ledMap.empty()) {
                colors = mapLedPositions(sampled, inner, dev.ledMap); // 精确坐标
            } else {
                colors = mapEdgeLeds(sampled, inner, dev.top, dev.right, dev.bottom, dev.left); // 四边
            }

            // 5) 全局后处理（UI 参数）+ 设备校正
            for (auto& c : colors) {
                c = applyColorTemperature(c, ui.colorTemperatureK); // TODO: 6500K≈单位增益
                c = applyHueOffset(c, ui.hueOffsetDeg);             // TODO: RGB↔HSL/HSV
                c = applySaturation(c, ui.saturationFactor);        // TODO: HSL: S*=factor
                c = applyWhiteBalance(c, dev.whiteBalance);         // 设备白平衡
                c = applyGamma(c, dev.gamma);                       // 设备 gamma
            }

            // 6) 平滑（指数移动平均：new = (1-α)*old + α*new）
            if (ui.smoothing > 0.0f) {
                auto& prev = lastFrameColor[dev.id];
                if (prev.size() != colors.size())
                    prev.assign(colors.size(), ColorRGBA{0,0,0,255});

                const float a = std::clamp(ui.smoothing, 0.0f, 1.0f);
                for (size_t i=0; i<colors.size(); ++i) {
                    colors[i].r = lerp(prev[i].r, colors[i].r, a);
                    colors[i].g = lerp(prev[i].g, colors[i].g, a);
                    colors[i].b = lerp(prev[i].b, colors[i].b, a);
                    colors[i].a = 255;
                }
                prev = colors;
            } else {
                lastFrameColor[dev.id] = colors;
            }

            // 7) 下发到设备（顺序与硬件一致）
            sendToDevice(dev.id, colors); // TODO: SPI/USB/串口/网口/SDK
        }

        // 8) 帧率控制
        sleepUntilNextFrame(ui.fps);
    }
}

// ================== 工具函数（示意） ==================

// 采样区域：按 edgeDistance 从四边内缩
Rect shrinkToInnerRect(const Rect& r, float edge) {
    edge = std::clamp(edge, 0.0f, 0.49f);
    int dx = int(r.width  * edge);
    int dy = int(r.height * edge);
    return Rect(r.x + dx, r.y + dy, r.width - 2*dx, r.height - 2*dy);
}

// 区域裁剪到屏幕范围
Rect clampToScreen(const Rect& rc, const Size& sz) {
    int x = std::clamp(rc.x, 0, sz.w);
    int y = std::clamp(rc.y, 0, sz.h);
    int w = std::clamp(rc.width,  0, sz.w - x);
    int h = std::clamp(rc.height, 0, sz.h - y);
    return Rect(x,y,w,h);
}

// 把归一化坐标(左下原点)映射为 inner 内像素(左上原点)
Point uvToPixel(const Rect& inner, const Size& imgSize, Vec2 uv) {
    float u = std::clamp(uv.x, 0.0f, 1.0f);
    float v = std::clamp(uv.y, 0.0f, 1.0f);
    int px = inner.x + int(u * (inner.width  - 1));
    int py = inner.y + int((1.0f - v) * (inner.height - 1)); // 左上为原点需翻转 y
    return {px, py};
}

std::vector<ColorRGBA> mapLedPositions(const Bitmap& img, const Rect& inner,
                                       const std::vector<Vec2>& ledMap) {
    std::vector<ColorRGBA> out; out.reserve(ledMap.size());
    for (auto uv : ledMap) {
        Point p = uvToPixel(inner, img.size(), uv);
        out.push_back(sampleAreaAvg(img, p, /*radius=*/1)); // TODO: 小邻域平均抗噪
    }
    return out;
}

std::vector<ColorRGBA> mapEdgeLeds(const Bitmap& img, const Rect& inner,
                                   int top, int right, int bottom, int left) {
    std::vector<ColorRGBA> out; out.reserve(top+right+bottom+left);

    auto lerpInt = [](int a,int b,float t){ return a + int((b-a)*t + 0.5f); };

    // 上边：左→右
    for (int i=0; i<top; ++i) {
        float t = (top==1)?0.5f: (float)i/(top-1);
        Point p{ lerpInt(inner.x, inner.x+inner.width-1, t), inner.y };
        out.push_back(sampleAreaAvg(img, p, 1));
    }
    // 右边：上→下
    for (int i=0; i<right; ++i) {
        float t = (right==1)?0.5f: (float)i/(right-1);
        Point p{ inner.x+inner.width-1, lerpInt(inner.y, inner.y+inner.height-1, t) };
        out.push_back(sampleAreaAvg(img, p, 1));
    }
    // 下边：右→左（示例按某些硬件顺序，如需可反向）
    for (int i=0; i<bottom; ++i) {
        float t = (bottom==1)?0.5f: (float)i/(bottom-1);
        Point p{ lerpInt(inner.x+inner.width-1, inner.x, t), inner.y+inner.height-1 };
        out.push_back(sampleAreaAvg(img, p, 1));
    }
    // 左边：下→上
    for (int i=0; i<left; ++i) {
        float t = (left==1)?0.5f: (float)i/(left-1);
        Point p{ inner.x, lerpInt(inner.y+inner.height-1, inner.y, t) };
        out.push_back(sampleAreaAvg(img, p, 1));
    }
    return out;
}

// ======= 颜色处理（示意：按你的项目替换具体实现） =======
ColorRGBA applyColorTemperature(ColorRGBA c, float kelvin) { /* TODO */ return c; }
ColorRGBA applyHueOffset(ColorRGBA c, float deg)           { /* TODO */ return c; }
ColorRGBA applySaturation(ColorRGBA c, float factor)       { /* TODO */ return c; }
ColorRGBA applyWhiteBalance(ColorRGBA c, const Vec3& wb)   { /* TODO */ return c; }
ColorRGBA applyGamma(ColorRGBA c, float gamma)             { /* TODO */ return c; }

// 采样、缩放、帧控等
Bitmap     downsample(const Bitmap& src, float ratio)      { /* TODO */ return src; }
ColorRGBA  sampleAreaAvg(const Bitmap& img, Point p, int r){ /* TODO */ return img.get(p); }
void       sendToDevice(const std::string& id, const std::vector<ColorRGBA>& c) { /* TODO */ }
void       sleepUntilNextFrame(int fps)                    { /* TODO */ }
