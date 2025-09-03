// 统一Ambilight：所有设备共享同一个区域
state.unifiedEffect = {
    type: Ambilight,
    monitorId: 0,
    captureZone: {x:0,y:0,w:screen.w,h:screen.h}, // 或UI提供的矩形
    edgeDistance: 0.06
}

// 设备独立 Ambilight：右键设备 → 设置本设备效果 → 为每个设备绑定不同 captureZone
device.localEffect = {
    type: Ambilight,
    monitorId: 1,
    captureZone: {x: 0, y: 0, w: screen.w/2