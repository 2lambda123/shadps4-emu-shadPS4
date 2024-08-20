// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <filesystem>
#include <thread>

#include "common/singleton.h"
#include "core/linker.h"
#include "input/controller.h"
#include "sdl_window.h"

namespace Core {

using HLEInitDef = void (*)(Core::Loader::SymbolsResolver* sym);

struct SysModules {
    std::string_view module_name;
    HLEInitDef callback;
};

class Emulator {
public:
    Emulator();
    ~Emulator();

    void Run(const std::filesystem::path& file);

private:
    void LoadSystemModules(const std::filesystem::path& file);

    Core::MemoryManager* memory;
    Input::GameController* controller;
    Core::Linker* linker;
    std::unique_ptr<Frontend::WindowSDL> window;
    std::map<u32, KeysMapping> m_keysBindingsMap;
};
} // namespace Core
