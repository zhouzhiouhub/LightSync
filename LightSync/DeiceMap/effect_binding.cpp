tick(dt, t): // dt:秒, t:全局时间
    if (state.useUnifiedEffect) computeOrdering()

    for idx in 0..state.devices.size-1:
        dev = state.devices[idx]
        if (dev.hasLocalEffect):
            colors = renderEffect(dev.localEffect, dev, dt, t, /*orderPhase=*/0, /*rank=*/0, /*rankCount=*/1)
        else if (state.useUnifiedEffect):
            rank = indexOf(state.orderedDeviceIdx, idx)           // 当前设备在排序中的名次
            rankCount = state.devices.size()
            orderPhase = float(rank) / max(1, rankCount-1)        // 0..1
            colors = renderEffect(state.unifiedEffect, dev, dt, t, orderPhase, rank, rankCount)
        else {
            // 没有本地也不使用统一 → 忽略或保留上帧
            continue
        }

        // 方向与旋转
        if (dev.orientation == Reverse) reverse(colors)
        colors = rotateByDevice(colors, dev.rotationDeg) // 如需将效果角度与摆放对齐

        dev.frame = colors
        sendToHardware(dev.id, colors)