// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <thread>
#include "common/assert.h"
#include "common/native_clock.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/kernel/time_management.h"
#include "core/libraries/libs.h"

#ifdef _WIN64
#include <pthread_time.h>
#else
#include <time.h>
#endif

namespace Libraries::Kernel {

static u64 initial_ptc;
static std::unique_ptr<Common::NativeClock> clock;

u64 PS4_SYSV_ABI sceKernelGetTscFrequency() {
    return clock->GetTscFrequency();
}

u64 PS4_SYSV_ABI sceKernelGetProcessTime() {
    return clock->GetProcessTimeUS();
}

u64 PS4_SYSV_ABI sceKernelGetProcessTimeCounter() {
    return clock->GetUptime() - initial_ptc;
}

u64 PS4_SYSV_ABI sceKernelGetProcessTimeCounterFrequency() {
    return clock->GetTscFrequency();
}

u64 PS4_SYSV_ABI sceKernelReadTsc() {
    return clock->GetUptime();
}

int PS4_SYSV_ABI sceKernelUsleep(u32 microseconds) {
    ASSERT(microseconds >= 1000);
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    return 0;
}

int PS4_SYSV_ABI posix_usleep(u32 microseconds) {
    ASSERT(microseconds >= 1000);
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    return 0;
}

u32 PS4_SYSV_ABI sceKernelSleep(u32 seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    return 0;
}

int PS4_SYSV_ABI sceKernelClockGettime(s32 clock_id, OrbisKernelTimespec* tp) {
    if (tp == nullptr) {
        return SCE_KERNEL_ERROR_EFAULT;
    }
    clockid_t pclock_id = CLOCK_REALTIME;
    switch (clock_id) {
    case 0:
        pclock_id = CLOCK_REALTIME;
        break;
    case 13:
    case 4:
        pclock_id = CLOCK_MONOTONIC;
        break;
    default:
        UNREACHABLE();
    }

    timespec t{};
    int result = clock_gettime(pclock_id, &t);
    tp->tv_sec = t.tv_sec;
    tp->tv_nsec = t.tv_nsec;
    if (result == 0) {
        return SCE_OK;
    }
    return SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI clock_gettime(s32 clock_id, OrbisKernelTimespec* time) {
    int result = sceKernelClockGettime(clock_id, time);
    if (result < 0) {
        UNREACHABLE(); // TODO return posix error code
    }
    return result;
}

int PS4_SYSV_ABI posix_nanosleep(const OrbisKernelTimespec* rqtp, OrbisKernelTimespec* rmtp) {
    const auto* request = reinterpret_cast<const timespec*>(rqtp);
    auto* remain = reinterpret_cast<timespec*>(rmtp);
    return nanosleep(request, remain);
}

int PS4_SYSV_ABI sceKernelNanosleep(const OrbisKernelTimespec* rqtp, OrbisKernelTimespec* rmtp) {
    if (!rqtp || !rmtp) {
        return SCE_KERNEL_ERROR_EFAULT;
    }

    if (rqtp->tv_sec < 0 || rqtp->tv_nsec < 0) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    return posix_nanosleep(rqtp, rmtp);
}

int PS4_SYSV_ABI sceKernelGettimeofday(OrbisKernelTimeval* tp) {
    if (!tp) {
        return ORBIS_KERNEL_ERROR_EFAULT;
    }

    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto microsecs = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);

    tp->tv_sec = seconds.count();
    tp->tv_usec = microsecs.count();
    return ORBIS_OK;
}

int PS4_SYSV_ABI gettimeofday(OrbisKernelTimeval* tp, OrbisKernelTimezone* tz) {
    // FreeBSD docs mention that the kernel generally does not track these values
    // and they	are usually returned as	zero.
    if (tz) {
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }
    return sceKernelGettimeofday(tp);
}

s32 PS4_SYSV_ABI sceKernelGettimezone(OrbisKernelTimezone* tz) {
    ASSERT(tz);
    static int tzflag = 0;
    if (!tzflag) {
        _tzset();
        tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
    return ORBIS_OK;
}

void timeSymbolsRegister(Core::Loader::SymbolsResolver* sym) {
    clock = std::make_unique<Common::NativeClock>();
    initial_ptc = clock->GetUptime();
    LIB_FUNCTION("4J2sUJmuHZQ", "libkernel", 1, "libkernel", 1, 1, sceKernelGetProcessTime);
    LIB_FUNCTION("fgxnMeTNUtY", "libkernel", 1, "libkernel", 1, 1, sceKernelGetProcessTimeCounter);
    LIB_FUNCTION("BNowx2l588E", "libkernel", 1, "libkernel", 1, 1,
                 sceKernelGetProcessTimeCounterFrequency);
    LIB_FUNCTION("-2IRUCO--PM", "libkernel", 1, "libkernel", 1, 1, sceKernelReadTsc);
    LIB_FUNCTION("1j3S3n-tTW4", "libkernel", 1, "libkernel", 1, 1, sceKernelGetTscFrequency);
    LIB_FUNCTION("ejekcaNQNq0", "libkernel", 1, "libkernel", 1, 1, sceKernelGettimeofday);
    LIB_FUNCTION("n88vx3C5nW8", "libkernel", 1, "libkernel", 1, 1, gettimeofday);
    LIB_FUNCTION("n88vx3C5nW8", "libScePosix", 1, "libkernel", 1, 1, gettimeofday);
    LIB_FUNCTION("1jfXLRVzisc", "libkernel", 1, "libkernel", 1, 1, sceKernelUsleep);
    LIB_FUNCTION("QcteRwbsnV0", "libScePosix", 1, "libkernel", 1, 1, posix_usleep);
    LIB_FUNCTION("-ZR+hG7aDHw", "libkernel", 1, "libkernel", 1, 1, sceKernelSleep);
    LIB_FUNCTION("0wu33hunNdE", "libScePosix", 1, "libkernel", 1, 1, sceKernelSleep);
    LIB_FUNCTION("yS8U2TGCe1A", "libkernel", 1, "libkernel", 1, 1, posix_nanosleep);
    LIB_FUNCTION("QBi7HCK03hw", "libkernel", 1, "libkernel", 1, 1, sceKernelClockGettime);
    LIB_FUNCTION("lLMT9vJAck0", "libkernel", 1, "libkernel", 1, 1, clock_gettime);
    LIB_FUNCTION("lLMT9vJAck0", "libScePosix", 1, "libkernel", 1, 1, clock_gettime);
    LIB_FUNCTION("kOcnerypnQA", "libkernel", 1, "libkernel", 1, 1, sceKernelGettimezone);
}

} // namespace Libraries::Kernel
