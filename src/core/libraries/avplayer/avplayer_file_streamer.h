// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "avplayer.h"
#include "avplayer_data_streamer.h"

#include <string_view>
#include <vector>

struct AVIOContext;

namespace Libraries::AvPlayer {

class AvPlayerFileStreamer : public IDataStreamer {
public:
    AvPlayerFileStreamer(const SceAvPlayerFileReplacement& file_replacement, std::string_view path);
    ~AvPlayerFileStreamer();

    AVIOContext* GetContext() override {
        return m_avio_context;
    }

private:
    static s32 ReadPacket(void* opaque, u8* buffer, s32 size);
    static s64 Seek(void* opaque, s64 buffer, int whence);

    SceAvPlayerFileReplacement m_file_replacement;

    int m_fd = -1;
    u64 m_position{};
    u64 m_file_size{};
    AVIOContext* m_avio_context{};
};

} // namespace Libraries::AvPlayer
