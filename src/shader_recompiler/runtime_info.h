// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <span>
#include <boost/container/static_vector.hpp>
#include "common/assert.h"
#include "common/types.h"
#include "shader_recompiler/ir/attribute.h"
#include "video_core/amdgpu/pixel_format.h"

namespace Shader {

static constexpr size_t NumUserDataRegs = 16;

enum class Stage : u32 {
    Vertex,
    TessellationControl,
    TessellationEval,
    Geometry,
    Fragment,
    Compute,
};
constexpr u32 MaxStageTypes = 6;

[[nodiscard]] constexpr Stage StageFromIndex(size_t index) noexcept {
    return static_cast<Stage>(static_cast<size_t>(Stage::Vertex) + index);
}

enum class TextureType : u32 {
    Color1D,
    ColorArray1D,
    Color2D,
    ColorArray2D,
    Color3D,
    ColorCube,
    Buffer,
};
constexpr u32 NUM_TEXTURE_TYPES = 7;

struct Info {
    explicit Info(std::span<const u32, 16> user_data_) : user_data{user_data_} {}

    struct VsInput {
        AmdGpu::NumberFormat fmt;
        u16 binding;
        u16 num_components;
    };
    boost::container::static_vector<VsInput, 32> vs_inputs{};

    struct PsInput {
        u32 param_index;
        u32 semantic;
        bool is_default;
        bool is_flat;
        u32 default_value;
    };
    boost::container::static_vector<PsInput, 32> ps_inputs{};

    struct AttributeFlags {
        bool Get(IR::Attribute attrib, u32 comp = 0) const {
            return flags[static_cast<size_t>(attrib)] & (1 << comp);
        }

        bool GetAny(IR::Attribute attrib) const {
            return flags[static_cast<size_t>(attrib)];
        }

        void Set(IR::Attribute attrib, u32 comp = 0) {
            flags[static_cast<size_t>(attrib)] |= (1 << comp);
        }

        u32 NumComponents(IR::Attribute attrib) const {
            const u8 mask = flags[static_cast<size_t>(attrib)];
            ASSERT(mask != 0b1011 || mask != 0b1101);
            return std::popcount(mask);
        }

        std::array<u8, IR::NumAttributes> flags;
    };
    AttributeFlags loads{};
    AttributeFlags stores{};

    std::span<const u32, 16> user_data;
    Stage stage;

    template <typename T>
    T ReadUd(u32 ptr_index, u32 dword_offset) const noexcept {
        T data;
        u32* base;
        std::memcpy(&base, &user_data[ptr_index], sizeof(base));
        std::memcpy(&data, base + dword_offset, sizeof(T));
        return data;
    }
};

} // namespace Shader
