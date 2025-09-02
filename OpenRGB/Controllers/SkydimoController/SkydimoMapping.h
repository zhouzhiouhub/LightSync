/*---------------------------------------------------------*\
| SkydimoMapping.h                                          |
|                                                           |
|   Matrix color reordering helpers for install direction   |
|   mapping.                                                |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkydimoInstallDirectionManager.h"
#include <vector>

namespace SkydimoMapping
{
    inline int ledIndexAt(const matrix_map_type* matrix, unsigned int x, unsigned int y)
    {
        if(matrix == nullptr)
        {
            return -1;
        }
        unsigned int width  = matrix->width;
        unsigned int height = matrix->height;
        if(x >= width || y >= height)
        {
            return -1;
        }
        unsigned int mapped = matrix->map[y * width + x];
        if(mapped == 0xFFFFFFFFu)
        {
            return -1;
        }
        return static_cast<int>(mapped);
    }

    inline void reorderColorsForMatrix(const matrix_map_type* matrix,
                                       const std::vector<RGBColor>& in,
                                       SkydimoInstallDirection direction,
                                       std::vector<RGBColor>& out)
    {
        if(matrix == nullptr)
        {
            out = in;
            return;
        }

        const unsigned int width  = matrix->width;
        const unsigned int height = matrix->height;
        // 先用原始输入填充输出，确保即便镜像后的源坐标无效，也不会遗漏写入
        out = in;
        out.resize(in.size());

        for(unsigned int y = 0; y < height; ++y)
        {
            for(unsigned int x = 0; x < width; ++x)
            {
                unsigned int srcX = x;
                unsigned int srcY = y;
                if(direction == INSTALL_FLIP_HORIZONTAL || direction == INSTALL_FLIP_BOTH)
                {
                    srcX = width - 1 - x;
                }
                if(direction == INSTALL_FLIP_VERTICAL || direction == INSTALL_FLIP_BOTH)
                {
                    srcY = height - 1 - y;
                }

                int srcLed = ledIndexAt(matrix, srcX, srcY);
                int dstLed = ledIndexAt(matrix, x, y);
                if(srcLed >= 0 && dstLed >= 0)
                {
                    out[dstLed] = in[srcLed];
                }
                // 若 src 无效但 dst 有效，保持 out[dstLed] 为原值（已由 out = in 设定）
            }
        }
    }
}


