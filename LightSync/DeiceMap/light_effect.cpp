computeOrdering():
    center = { state.canvas.x + state.canvas.w/2, state.canvas.y + state.canvas.h/2 }
    vec<pair<int,float>> key; // (deviceIndex, keyValue)

    switch (state.orderMode):
      case LeftToRight:
          for i,dev in enumerate(state.devices):
              cx = dev.bounds.x + dev.bounds.w/2
              key.push_back({i, cx})
          sort key by keyValue ascending
          break

      case CenterToOutside:
          for i,dev:
              cx = dev.bounds.x + dev.bounds.w/2
              cy = dev.bounds.y + dev.bounds.h/2
              d = distance( {cx,cy}, center )
              key.push_back({i, d})
          sort key by keyValue ascending     // 近→远
          break

      case OutsideToCenter:
          for i,dev:
              cx = dev.bounds.x + dev.bounds.w/2
              cy = dev.bounds.y + dev.bounds.h/2
              d = distance( {cx,cy}, center )
              key.push_back({i, d})
          sort key by keyValue descending    // 远→近
          break

    state.orderedDeviceIdx = [p.first for p in key]