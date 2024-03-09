// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "common/types.h"

namespace Core::Loader {

enum class SymbolType {
    Unknown,
    Function,
    Object,
    Tls,
    NoType,
};

struct SymbolRecord {
    std::string name;
    u64 virtual_address;
};

struct SymbolResolver {
    std::string name;
    std::string nidName;
    std::string library;
    u16 library_version;
    std::string module;
    u8 module_version_major;
    u8 module_version_minor;
    SymbolType type;
};

class SymbolsResolver {
public:
    SymbolsResolver() = default;
    virtual ~SymbolsResolver() = default;

    void AddSymbol(const SymbolResolver& s, u64 virtual_addr);
    const SymbolRecord* FindSymbol(const SymbolResolver& s) const;

    static std::string GenerateName(const SymbolResolver& s);

private:
    std::vector<SymbolRecord> m_symbols;
};

} // namespace Core::Loader
