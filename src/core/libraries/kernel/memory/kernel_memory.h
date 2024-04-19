// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::Kernel {
s32 PS4_SYSV_ABI sceKernelMapNamedFlexibleMemory(void** addrInOut, std::size_t len, int prot,
                                                 int flags, const char* name);

void RegisterKernelMemory(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::Kernel