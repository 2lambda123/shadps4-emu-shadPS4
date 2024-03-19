// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <mutex>

#include "src/common/types.h"

class SDLAudio {
public:
    SDLAudio() = default;
    virtual ~SDLAudio() = default;

    int AudioInit();
    int AudioOutOpen(int type, u32 samples_num, u32 freq, u32 format);

private:
    struct PortOut {
        bool isOpen = false;
        int type = 0;
        u32 samples_num = 0;
        u32 freq = 0;
        u32 format = -1;
        int channels_num = 0;
        int volume[8] = {};
    };
    std::mutex m_mutex;
    std::array<PortOut, 8> portsOut; // main support up to 8 ports
};