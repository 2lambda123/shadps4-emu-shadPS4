// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>
#include "common/types.h"

namespace Libraries::Kernel {

constexpr s16 EVFILT_READ = -1;
constexpr s16 EVFILT_WRITE = -2;
constexpr s16 EVFILT_AIO = -3;    // attached to aio requests
constexpr s16 EVFILT_VNODE = -4;  // attached to vnodes
constexpr s16 EVFILT_PROC = -5;   // attached to struct proc
constexpr s16 EVFILT_SIGNAL = -6; // attached to struct proc
constexpr s16 EVFILT_TIMER = -7;  // timers
constexpr s16 EVFILT_FS = -9;     // filesystem events
constexpr s16 EVFILT_LIO = -10;   // attached to lio requests
constexpr s16 EVFILT_USER = -11;  // User events
constexpr s16 EVFILT_POLLING = -12;
constexpr s16 EVFILT_VIDEO_OUT = -13;
constexpr s16 EVFILT_GRAPHICS_CORE = -14;
constexpr s16 EVFILT_HRTIMER = -15;
constexpr s16 EVFILT_UVD_TRAP = -16;
constexpr s16 EVFILT_VCE_TRAP = -17;
constexpr s16 EVFILT_SDMA_TRAP = -18;
constexpr s16 EVFILT_REG_EV = -19;
constexpr s16 EVFILT_GPU_EXCEPTION = -20;
constexpr s16 EVFILT_GPU_SYSTEM_EXCEPTION = -21;
constexpr s16 EVFILT_GPU_DBGGC_EV = -22;
constexpr s16 EVFILT_SYSCOUNT = 22;

constexpr u16 EV_ONESHOT = 0x10;		// only report one occurrence
constexpr u16 EV_CLEAR = 0x20;		// clear event state after reporting
constexpr u16 EV_RECEIPT = 0x40;		// force EV_ERROR on success, data=0
constexpr u16 EV_DISPATCH = 0x80;		// disable event after reporting
constexpr u16 EV_SYSFLAGS = 0xF000;		// reserved by system
constexpr u16 EV_FLAG1 = 0x2000;		// filter-specific flag

class EqueueInternal;
struct EqueueEvent;

using TriggerFunc = void (*)(EqueueEvent* event, void* trigger_data);
using ResetFunc = void (*)(EqueueEvent* event);
using DeleteFunc = void (*)(EqueueInternal* eq, EqueueEvent* event);

struct SceKernelEvent {
    enum Type : u64 {
        Compute0RelMem = 0x00,
        Compute1RelMem = 0x01,
        Compute2RelMem = 0x02,
        Compute3RelMem = 0x03,
        Compute4RelMem = 0x04,
        Compute5RelMem = 0x05,
        Compute6RelMem = 0x06,
        GfxEop = 0x40
    };

    u64 ident = 0;  /* identifier for this event */
    s16 filter = 0; /* filter for event */
    u16 flags = 0;
    u32 fflags = 0;
    u64 data = 0;
    void* udata = nullptr; /* opaque user data identifier */
};

struct Filter {
    void* data = nullptr;
};

struct EqueueEvent {
    bool isTriggered = false;
    SceKernelEvent event;
    Filter filter;

    void reset() {
        isTriggered = false;
        event.fflags = 0;
        event.data = 0;
    }

    void trigger(void* data) {
        isTriggered = true;
        event.fflags++;
        event.data = reinterpret_cast<uintptr_t>(data);
    }
};

class EqueueInternal {
public:
    EqueueInternal() = default;
    virtual ~EqueueInternal();
    void setName(const std::string& m_name) {
        this->m_name = m_name;
    }
    int addEvent(const EqueueEvent& event);
    int removeEvent(u64 id);
    int waitForEvents(SceKernelEvent* ev, int num, u32 micros);
    bool triggerEvent(u64 ident, s16 filter, void* trigger_data);
    int getTriggeredEvents(SceKernelEvent* ev, int num);

private:
    std::string m_name;
    std::mutex m_mutex;
    std::vector<EqueueEvent> m_events;
    std::condition_variable m_cond;
};

} // namespace Libraries::Kernel
