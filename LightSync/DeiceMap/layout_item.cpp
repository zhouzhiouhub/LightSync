// 鼠标交互
onMouseDown(pos):
    hit = pickDeviceAt(pos)
    if hit:
        drag.active = true
        drag.deviceIndex = hit.index
        drag.offset = pos - device.bounds.topLeft

onMouseMove(pos):
    if drag.active:
        device = state.devices[drag.deviceIndex]
        device.bounds.x = pos.x - drag.offset.x
        device.bounds.y = pos.y - drag.offset.y
        requestRepaint()

onMouseUp(pos):
    drag.active = false
    // 可选：吸附到网格/边缘
    snapToGrid(state.devices[drag.deviceIndex])

// 右键菜单：设置“本设备效果”或“使用统一效果”
onContextMenu(deviceIndex):
    showMenu([
      "设置本设备效果", () => openEffectDialog(deviceIndex),
      "使用统一效果", () => state.devices[deviceIndex].hasLocalEffect=false
    ])