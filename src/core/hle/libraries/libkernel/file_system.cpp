#include "common/log.h"
#include "common/debug.h"
#include "core/hle/libraries/libkernel/file_system.h"
#include "core/hle/libraries/libs.h"

namespace Core::Libraries::LibKernel {

constexpr bool log_file_fs = true;  // disable it to disable logging

int PS4_SYSV_ABI sceKernelOpen(const char* path, int flags, u16 mode) {
    LOG_INFO_IF(log_file_fs, "sceKernelOpen path = {} flags = {:#x} mode = {:#x}\n", path, flags, mode);
    return 0;
}

int PS4_SYSV_ABI posix_open(const char* path, int flags, /* SceKernelMode*/ u16 mode) {
    LOG_INFO_IF(log_file_fs, "posix open redirect to sceKernelOpen\n");
    int result = sceKernelOpen(path, flags, mode);
    if (result < 0) {
        BREAKPOINT();  // posix calls different only for their return values
    }
    return result;
}

void fileSystemSymbolsRegister(Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("1G3lF1Gg1k8", "libkernel", 1, "libkernel", 1, 1, sceKernelOpen);
    LIB_FUNCTION("wuCroIGjt2g", "libScePosix", 1, "libkernel", 1, 1, posix_open);

    //openOrbis (to check if it is valid out of OpenOrbis
    LIB_FUNCTION("6c3rCVE-fTU", "libkernel", 1, "libkernel", 1, 1, posix_open);// _open shoudld be equal to open function
}

} // namespace Core::Libraries::LibKernel
