#include "common/debug.h"
#include "common/log.h"
#include "core/hle/libraries/libc/libc_stdio.h"

namespace Core::Libraries::LibC {

constexpr bool log_file_libc = true;  // disable it to disable logging

int PS4_SYSV_ABI printf(VA_ARGS) {
    VA_CTX(ctx);
    return printf_ctx(&ctx);
}

int PS4_SYSV_ABI fprintf(FILE* file, VA_ARGS) {
    int fd = _fileno(file);
    if (fd == 1 || fd == 2) {  // output stdout and stderr to console
        VA_CTX(ctx);
        return printf_ctx(&ctx);
    }
    LOG_ERROR_IF(log_file_libc, "libc:Unimplemented fprintf case\n");
    BREAKPOINT();
    return 0;
}

int PS4_SYSV_ABI snprintf(char* s, size_t n, VA_ARGS) { 
    VA_CTX(ctx);
    return snprintf_ctx(s, n, &ctx);
}

int PS4_SYSV_ABI sprintf(char* s,VA_ARGS) {
    VA_CTX(ctx);
    return sprintf_ctx(s,&ctx);
}

int PS4_SYSV_ABI vsnprintf(char* s, size_t n, const char* format, VaList* arg) {
    return vsnprintf_ctx(s, n, format, arg);
}

int PS4_SYSV_ABI puts(const char* s) {
    return std::puts(s);
}

} // namespace Core::Libraries::LibC
