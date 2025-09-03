vector<Color> renderEffect(EffectPreset e, DeviceNode dev, float dt, float t,
    float orderPhase, int rank, int rankCount)
{
switch (e.type):

case Solid:
return fill(dev.leds, hsv2rgb({e.hueShift, e.saturation, e.brightness}))

case Gradient:
// 统一效果：整排设备视为一条“总带”，orderPhase决定本设备在总带的位置
// 计算本设备起止全局区间，用于取渐变
start = orderPhase
span  = 1.0f / rankCount
return sampleGradientOverSpan(dev.leds, start, span, e, t)

case Rainbow:
// 相位 = 设备排序名次，形成左→右/中心→外的相位差
base = t * e.speed + orderPhase
return perLed(dev.leds, i -> hsv2rgb({ base + i/dev.leds, e.saturation, e.brightness }))

case Breathe:
amp = 0.5f + 0.5f * sin(TAU*(t*e.speed + orderPhase))
return fill(dev.leds, hsv2rgb({ e.hueShift, e.saturation, amp*e.brightness }))

case Comet:
// 一个“彗星”沿全局顺序移动；orderPhase决定该设备上的彗星位置
head = fract(t * e.speed)            // 0..1
// 把全局 head 投影到本设备区间
start = orderPhase
span  = 1.0f / rankCount
headLocal = (head - start) / span    // 本设备局部0..1
return drawComet(dev.leds, headLocal, e)

case Ambilight:
// e.captureZone: 每个设备可不同（若本地效果），或统一效果时统一区域
bitmap = captureScreen(e.monitorId, e.captureZone, e.edgeDistance)
return mapAmbilight(bitmap, dev)     // 利用dev.ledMap或四边均匀

case AudioReactive:
energy = getBandEnergy(/*params*/)   // 外部音频输入
phase  = t * e.speed + orderPhase
return audioColors(dev.leds, energy, phase, e)

return fill(dev.leds, {0,0,0})
}