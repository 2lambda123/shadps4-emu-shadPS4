// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/io_file.h"
#include "common/types.h"
#include "loader.h"

FileTypes detectFileType(const std::string& filepath) {
    if (filepath.size() == 0) // no file loaded
    {
        return FILETYPE_UNKNOWN;
    }
    Common::FS::IOFile file;
    file.Open(filepath, Common::FS::FileAccessMode::Read);
    file.Seek(0);
    u32 magic;
    file.ReadRaw<u8>(&magic, sizeof(magic));
    file.Close();
    switch (magic) {
    case 0x544e437f: // PS4 PKG
        return FILETYPE_PKG;
    }
    return FILETYPE_UNKNOWN;
}
