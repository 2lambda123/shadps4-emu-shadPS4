// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include "common/assert.h"
#include "common/config.h"
#include "common/version.h"
#include "core/libraries/pad/pad.h"
#include "input/controller.h"
#include "sdl_window.h"
#include "video_core/renderdoc.h"

#ifdef __APPLE__
#include <SDL3/SDL_metal.h>
#endif

namespace Frontend {

WindowSDL::WindowSDL(s32 width_, s32 height_, Input::GameController* controller_,
                     std::string_view window_title)
    : width{width_}, height{height_}, controller{controller_} {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        UNREACHABLE_MSG("Failed to initialize SDL video subsystem: {}", SDL_GetError());
    }
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                          std::string(window_title).c_str());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
    SDL_SetNumberProperty(props, "flags", SDL_WINDOW_VULKAN);
    window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (window == nullptr) {
        UNREACHABLE_MSG("Failed to create window handle: {}", SDL_GetError());
    }

    SDL_SetWindowFullscreen(window, Config::isFullscreenMode());

    SDL_InitSubSystem(SDL_INIT_GAMEPAD);
    controller->TryOpenSDLController();

#if defined(SDL_PLATFORM_WIN32)
    window_info.type = WindowSystemType::Windows;
    window_info.render_surface = SDL_GetPointerProperty(SDL_GetWindowProperties(window),
                                                        SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        window_info.type = WindowSystemType::X11;
        window_info.display_connection = SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        window_info.render_surface = (void*)SDL_GetNumberProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        window_info.type = WindowSystemType::Wayland;
        window_info.display_connection = SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        window_info.render_surface = SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
    }
#elif defined(SDL_PLATFORM_MACOS)
    window_info.type = WindowSystemType::Metal;
    window_info.render_surface = SDL_Metal_GetLayer(SDL_Metal_CreateView(window));
#endif
}

WindowSDL::~WindowSDL() = default;

void WindowSDL::waitEvent() {
    // Called on main thread
    SDL_Event event;

    if (!SDL_WaitEvent(&event)) {
        return;
    }

    switch (event.type) {
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
        onResize();
        break;
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WINDOW_EXPOSED:
        is_shown = event.type == SDL_EVENT_WINDOW_EXPOSED;
        onResize();
        break;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        onKeyPress(&event);
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        onGamepadEvent(&event);
        break;
    case SDL_EVENT_QUIT:
        is_open = false;
        break;
    default:
        break;
    }
}

void WindowSDL::setKeysBindingsMap(const std::map<u32, KeysMapping>& bindingsMap) {
    keysBindingsMap = bindingsMap;
}

void WindowSDL::onResize() {
    SDL_GetWindowSizeInPixels(window, &width, &height);
}

using Libraries::Pad::OrbisPadButtonDataOffset;

void WindowSDL::onKeyPress(const SDL_Event* event) {
    u32 button = 0;
    Input::Axis axis = Input::Axis::AxisMax;
    int axisvalue = 0;
    int ax = 0;

    bool keyHandlingPending = true;
    if (!keysBindingsMap.empty()) {

        std::optional<KeysMapping> ps4KeyOpt;
        auto foundIt = keysBindingsMap.find(event->key.key);
        if (foundIt != keysBindingsMap.end()) {
            ps4KeyOpt = foundIt->second;
        }

        // No support for modifiers (yet)
        if (ps4KeyOpt.has_value() && (event->key.mod == SDL_KMOD_NONE)) {
            keyHandlingPending = false;

            auto ps4Key = ps4KeyOpt.value();
            if (ps4Key == KeysMapping::Start_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_OPTIONS;
            if (ps4Key == KeysMapping::Triangle_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TRIANGLE;
            if (ps4Key == KeysMapping::Circle_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CIRCLE;
            if (ps4Key == KeysMapping::Cross_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CROSS;
            if (ps4Key == KeysMapping::Square_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_SQUARE;
            if (ps4Key == KeysMapping::R1_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R1;
            if (ps4Key == KeysMapping::R2_Key)
                handleR2Key(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::DPadLeft_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_LEFT;
            if (ps4Key == KeysMapping::DPadRight_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_RIGHT;
            if (ps4Key == KeysMapping::DPadDown_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_DOWN;
            if (ps4Key == KeysMapping::DPadUp_Key)
                button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_UP;
            if (ps4Key == KeysMapping::LAnalogLeft_Key)
                handleLAnalogLeftKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::LAnalogUp_Key)
                handleLAnalogUpKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::LAnalogDown_Key)
                handleLAnalogDownKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::RAnalogLeft_Key)
                handleRAnalogLeftKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::RAnalogRight_Key)
                handleRAnalogRightKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::RAnalogUp_Key)
                handleRAnalogUpKey(event, button, axis, axisvalue, ax);
            if (ps4Key == KeysMapping::RAnalogDown_Key)
                handleRAnalogDownKey(event, button, axis, axisvalue, ax);
        }
    }

    if (keyHandlingPending) {
        switch (event->key.key) {
        case SDLK_UP:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_UP;
            break;
        case SDLK_DOWN:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_DOWN;
            break;
        case SDLK_LEFT:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_LEFT;
            break;
        case SDLK_RIGHT:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_RIGHT;
            break;
        case Triangle_Key:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TRIANGLE;
            break;
        case Circle_Key:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CIRCLE;
            break;
        case Cross_Key:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CROSS;
            break;
        case Square_Key:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_SQUARE;
            break;
        case SDLK_KP_8:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TRIANGLE;
            break;
        case SDLK_KP_6:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CIRCLE;
            break;
        case SDLK_KP_2:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CROSS;
            break;
        case SDLK_KP_4:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_SQUARE;
            break;
        case SDLK_RETURN:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_OPTIONS;
            break;
        case SDLK_A:
            handleLAnalogLeftKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_D:
            handleLAnalogRightKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_W:
            handleLAnalogUpKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_S:
            handleLAnalogDownKey(event, button, axis, axisvalue, ax);
            if (event->key.mod == SDL_KMOD_LCTRL) {
                // Trigger rdoc capture
                VideoCore::TriggerCapture();
            }
            break;
        case SDLK_J:
            handleRAnalogLeftKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_L:
            handleRAnalogRightKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_I:
            handleRAnalogUpKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_K:
            handleRAnalogDownKey(event, button, axis, axisvalue, ax);
            break;
        case SDLK_X:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_L3;
            break;
        case SDLK_M:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R3;
            break;
        case SDLK_Q:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_L1;
            break;
        case SDLK_U:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R1;
            break;
        case SDLK_E:
            handleL2Key(event, button, axis, axisvalue, ax);
            break;
        case SDLK_O:
            handleR2Key(event, button, axis, axisvalue, ax);
            break;
        case SDLK_SPACE:
            button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TOUCH_PAD;
            break;
        default:
            break;
        }
    }

    if (button != 0) {
        controller->CheckButton(0, button, event->type == SDL_EVENT_KEY_DOWN);
    }
    if (axis != Input::Axis::AxisMax) {
        controller->Axis(0, axis, ax);
    }
}

void WindowSDL::handleR2Key(const SDL_Event* event, u32& button, Input::Axis& axis, int& axisvalue,
                            int& ax) {
    button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R2;
    axis = Input::Axis::TriggerRight;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 255;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(0, 0x80, axisvalue);
}

void WindowSDL::handleL2Key(const SDL_Event* event, u32& button, Input::Axis& axis, int& axisvalue,
                            int& ax) {
    button = OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_L2;
    axis = Input::Axis::TriggerLeft;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 255;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(0, 0x80, axisvalue);
}

void WindowSDL::handleLAnalogRightKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                      int& axisvalue, int& ax) {
    axis = Input::Axis::LeftX;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleLAnalogLeftKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                     int& axisvalue, int& ax) {
    axis = Input::Axis::LeftX;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += -127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleLAnalogUpKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                   int& axisvalue, int& ax) {
    axis = Input::Axis::LeftY;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += -127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleLAnalogDownKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                     int& axisvalue, int& ax) {
    axis = Input::Axis::LeftY;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleRAnalogRightKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                      int& axisvalue, int& ax) {
    axis = Input::Axis::RightX;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleRAnalogLeftKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                     int& axisvalue, int& ax) {
    axis = Input::Axis::RightX;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += -127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleRAnalogUpKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                   int& axisvalue, int& ax) {
    axis = Input::Axis::RightY;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += -127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::handleRAnalogDownKey(const SDL_Event* event, u32& button, Input::Axis& axis,
                                     int& axisvalue, int& ax) {
    axis = Input::Axis::RightY;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        axisvalue += 127;
    } else {
        axisvalue = 0;
    }
    ax = Input::GetAxis(-0x80, 0x80, axisvalue);
}

void WindowSDL::onGamepadEvent(const SDL_Event* event) {
    using Libraries::Pad::OrbisPadButtonDataOffset;

    u32 button = 0;
    Input::Axis axis = Input::Axis::AxisMax;
    switch (event->type) {
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        button = sdlGamepadToOrbisButton(event->gbutton.button);
        if (button != 0) {
            controller->CheckButton(0, button, event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN);
        }
        break;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        axis = event->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX           ? Input::Axis::LeftX
               : event->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY         ? Input::Axis::LeftY
               : event->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX        ? Input::Axis::RightX
               : event->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY        ? Input::Axis::RightY
               : event->gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER  ? Input::Axis::TriggerLeft
               : event->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER ? Input::Axis::TriggerRight
                                                                     : Input::Axis::AxisMax;
        if (axis != Input::Axis::AxisMax) {
            controller->Axis(0, axis, Input::GetAxis(-0x8000, 0x8000, event->gaxis.value));
        }
        break;
    }
}

int WindowSDL::sdlGamepadToOrbisButton(u8 button) {
    using Libraries::Pad::OrbisPadButtonDataOffset;

    switch (button) {
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_DOWN;
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_UP;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_LEFT;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_RIGHT;
    case SDL_GAMEPAD_BUTTON_SOUTH:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CROSS;
    case SDL_GAMEPAD_BUTTON_NORTH:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TRIANGLE;
    case SDL_GAMEPAD_BUTTON_WEST:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_SQUARE;
    case SDL_GAMEPAD_BUTTON_EAST:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_CIRCLE;
    case SDL_GAMEPAD_BUTTON_START:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_OPTIONS;
    case SDL_GAMEPAD_BUTTON_TOUCHPAD:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TOUCH_PAD;
    case SDL_GAMEPAD_BUTTON_BACK:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_TOUCH_PAD;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_L1;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R1;
    case SDL_GAMEPAD_BUTTON_LEFT_STICK:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_L3;
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
        return OrbisPadButtonDataOffset::ORBIS_PAD_BUTTON_R3;
    default:
        return 0;
    }
}

} // namespace Frontend
