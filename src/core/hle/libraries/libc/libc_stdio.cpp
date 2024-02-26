// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"
#include "core/hle/libraries/libc/libc_stdio.h"

namespace Core::Libraries::LibC {

int PS4_SYSV_ABI ps4_printf(VA_ARGS) {
    VA_CTX(ctx);
    return printf_ctx(&ctx);
}

int PS4_SYSV_ABI ps4_fprintf(FILE* file, VA_ARGS) {
    int fd = fileno(file);
    if (fd == 1 || fd == 2) { // output stdout and stderr to console
        VA_CTX(ctx);
        return printf_ctx(&ctx);
    }
    UNREACHABLE_MSG("Unimplemented fprintf case");
    return 0;
}

int PS4_SYSV_ABI ps4_vsnprintf(char* s, size_t n, const char* format, VaList* arg) {
    return vsnprintf_ctx(s, n, format, arg);
}

int PS4_SYSV_ABI ps4_puts(const char* s) {
    return std::puts(s);
}

} // namespace Core::Libraries::LibC
