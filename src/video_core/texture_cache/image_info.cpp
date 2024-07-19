// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"
#include "common/config.h"
#include "common/math.h"
#include "video_core/renderer_vulkan/liverpool_to_vk.h"
#include "video_core/texture_cache/image_info.h"

namespace VideoCore {

using namespace Vulkan;
using Libraries::VideoOut::TilingMode;
using VideoOutFormat = Libraries::VideoOut::PixelFormat;

static vk::Format ConvertPixelFormat(const VideoOutFormat format) {
    switch (format) {
    case VideoOutFormat::A8R8G8B8Srgb:
        return vk::Format::eB8G8R8A8Srgb;
    case VideoOutFormat::A8B8G8R8Srgb:
        return vk::Format::eR8G8B8A8Srgb;
    case VideoOutFormat::A2R10G10B10:
    case VideoOutFormat::A2R10G10B10Srgb:
        return vk::Format::eA2R10G10B10UnormPack32;
    default:
        break;
    }
    UNREACHABLE_MSG("Unknown format={}", static_cast<u32>(format));
    return {};
}

static vk::ImageType ConvertImageType(AmdGpu::ImageType type) noexcept {
    switch (type) {
    case AmdGpu::ImageType::Color1D:
    case AmdGpu::ImageType::Color1DArray:
        return vk::ImageType::e1D;
    case AmdGpu::ImageType::Color2D:
    case AmdGpu::ImageType::Cube:
    case AmdGpu::ImageType::Color2DArray:
        return vk::ImageType::e2D;
    case AmdGpu::ImageType::Color3D:
        return vk::ImageType::e3D;
    default:
        UNREACHABLE();
    }
}

// clang-format off
static constexpr std::array macro_tile_extents{
    std::pair{256u, 128u}, std::pair{256u, 128u}, std::pair{256u, 128u}, std::pair{256u, 128u}, // 00
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 128u}, std::pair{128u, 128u}, // 01
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 02
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 03
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 04
    std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     // 05
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 128u}, // 06
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 07
    std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     // 08
    std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     // 09
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 0A
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 0B
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 0C
    std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     // 0D
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 0E
    std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{128u, 64u},  // 0F
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 10
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 11
    std::pair{256u, 256u}, std::pair{256u, 128u}, std::pair{128u, 128u}, std::pair{128u, 64u},  // 12
    std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     std::pair{0u, 0u},     // 13
    std::pair{128u, 64u},  std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   // 14
    std::pair{128u, 64u},  std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   // 15
    std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   // 16
    std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   // 17
    std::pair{128u, 128u}, std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   // 18
    std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   std::pair{64u, 64u},   // 19
    std::pair{128u, 64u},  std::pair{64u, 64u},   std::pair{64u, 64u},   std::pair{64u, 64u},   // 1A
};
// clang-format on

static constexpr std::pair<u32, u32> GetMacroTileExtents(u32 tiling_idx, u32 bpp, u32 num_samples) {
    ASSERT(num_samples == 1);
    return macro_tile_extents[tiling_idx * 4 + IntLog2(bpp) - 3];
}

static constexpr size_t ImageSizeLinearAligned(u32 pitch, u32 height, u32 bpp, u32 num_samples) {
    const auto pitch_align = std::max(8u, 64u / ((bpp + 7) / 8));
    auto pitch_aligned = (pitch + pitch_align - 1) & ~(pitch_align - 1);
    const auto height_aligned = height;
    size_t log_sz = 1;
    const auto slice_align = std::max(64u, 256u / (bpp + 7) / 8);
    while (log_sz % slice_align) {
        log_sz = pitch_aligned * height_aligned * num_samples;
        pitch_aligned += pitch_align;
    }
    return (log_sz * bpp + 7) / 8;
}

static constexpr size_t ImageSizeMicroTiled(u32 pitch, u32 height, u32 bpp, u32 num_samples) {
    const auto pitch_align = 8u;
    const auto height_align = 8u;
    auto pitch_aligned = (pitch + pitch_align - 1) & ~(pitch_align - 1);
    const auto height_aligned = (height + height_align - 1) & ~(height_align - 1);
    size_t log_sz = 1;
    while (log_sz % 256) {
        log_sz = (pitch_aligned * height_aligned * bpp * num_samples + 7) / 8;
        pitch_aligned += 8;
    }
    return log_sz;
}

static constexpr size_t ImageSizeMacroTiled(u32 pitch, u32 height, u32 bpp, u32 num_samples,
                                            u32 tiling_idx) {
    const auto& [pitch_align, height_align] = GetMacroTileExtents(tiling_idx, bpp, num_samples);
    ASSERT(pitch_align != 0 && height_align != 0);
    const auto pitch_aligned = (pitch + pitch_align - 1) & ~(pitch_align - 1);
    const auto height_aligned = (height + height_align - 1) & ~(height_align - 1);
    return (pitch_aligned * height_aligned * bpp * num_samples + 7) / 8;
}

ImageInfo::ImageInfo(const Libraries::VideoOut::BufferAttributeGroup& group,
                     VAddr cpu_address) noexcept {
    const auto& attrib = group.attrib;
    is_tiled = attrib.tiling_mode == TilingMode::Tile;
    tiling_mode =
        is_tiled ? AmdGpu::TilingMode::Display_MacroTiled : AmdGpu::TilingMode::Display_Linear;
    pixel_format = ConvertPixelFormat(attrib.pixel_format);
    type = vk::ImageType::e2D;
    size.width = attrib.width;
    size.height = attrib.height;
    pitch = attrib.tiling_mode == TilingMode::Linear ? size.width : (size.width + 127) & (~127);
    const bool is_32bpp = attrib.pixel_format != VideoOutFormat::A16R16G16B16Float;
    ASSERT(is_32bpp);

    guest_address = cpu_address;
    if (!is_tiled) {
        guest_size_bytes = pitch * size.height * 4;
    } else {
        if (Config::isNeoMode()) {
            guest_size_bytes = pitch * ((size.height + 127) & (~127)) * 4;
        } else {
            guest_size_bytes = pitch * ((size.height + 63) & (~63)) * 4;
        }
    }
    usage.vo_buffer = true;
}

ImageInfo::ImageInfo(const AmdGpu::Liverpool::ColorBuffer& buffer,
                     const AmdGpu::Liverpool::CbDbExtent& hint /*= {}*/) noexcept {
    is_tiled = buffer.IsTiled();
    tiling_mode = buffer.GetTilingMode();
    pixel_format = LiverpoolToVK::SurfaceFormat(buffer.info.format, buffer.NumFormat());
    num_samples = 1 << buffer.attrib.num_fragments_log2;
    type = vk::ImageType::e2D;
    size.width = hint.Valid() ? hint.width : buffer.Pitch();
    size.height = hint.Valid() ? hint.height : buffer.Height();
    size.depth = 1;
    pitch = buffer.Pitch();
    resources.layers = buffer.NumSlices();
    meta_info.cmask_addr = buffer.info.fast_clear ? buffer.CmaskAddress() : 0;
    meta_info.fmask_addr = buffer.info.compression ? buffer.FmaskAddress() : 0;
    usage.render_target = true;

    guest_address = buffer.Address();
    guest_size_bytes = buffer.GetColorSliceSize() * buffer.NumSlices();
}

ImageInfo::ImageInfo(const AmdGpu::Liverpool::DepthBuffer& buffer, u32 num_slices,
                     VAddr htile_address, const AmdGpu::Liverpool::CbDbExtent& hint) noexcept {
    is_tiled = false;
    pixel_format = LiverpoolToVK::DepthFormat(buffer.z_info.format, buffer.stencil_info.format);
    type = vk::ImageType::e2D;
    num_samples = 1 << buffer.z_info.num_samples; // spec doesn't say it is a log2
    size.width = hint.Valid() ? hint.width : buffer.Pitch();
    size.height = hint.Valid() ? hint.height : buffer.Height();
    size.depth = 1;
    pitch = size.width;
    resources.layers = num_slices;
    meta_info.htile_addr = buffer.z_info.tile_surface_en ? htile_address : 0;
    usage.depth_target = true;

    guest_address = buffer.Address();
    guest_size_bytes = buffer.GetDepthSliceSize() * num_slices;
}

ImageInfo::ImageInfo(const AmdGpu::Image& image) noexcept {
    is_tiled = image.IsTiled();
    tiling_mode = image.GetTilingMode();
    pixel_format = LiverpoolToVK::SurfaceFormat(image.GetDataFmt(), image.GetNumberFmt());
    type = ConvertImageType(image.GetType());
    is_cube = image.GetType() == AmdGpu::ImageType::Cube;
    const auto is_volume = image.GetType() == AmdGpu::ImageType::Color3D;
    size.width = image.width + 1;
    size.height = image.height + 1;
    size.depth = is_volume ? image.depth + 1 : 1;
    pitch = image.Pitch();
    resources.levels = image.NumLevels();
    resources.layers = image.NumLayers();
    usage.texture = true;

    guest_address = image.Address();
    guest_size_bytes = image.GetSize();

    mips_layout.reserve(resources.levels);
    const auto num_bits = NumBits(image.GetDataFmt());
    const auto is_block = IsBlockCoded();
    const auto is_pow2 = image.pow2pad;

    guest_size_bytes = 0;
    for (auto mip = 0u; mip < resources.levels; ++mip) {
        auto bpp = num_bits;
        auto mip_w = pitch >> mip;
        auto mip_h = size.height >> mip;
        if (is_block) {
            mip_w = (mip_w + 3) / 4;
            mip_h = (mip_h + 3) / 4;
            bpp *= 16;
        }
        mip_w = std::max(mip_w, 1u);
        mip_h = std::max(mip_h, 1u);
        auto mip_d = std::max(size.depth >> mip, 1u);

        if (is_pow2) {
            mip_w = std::bit_ceil(mip_w);
            mip_h = std::bit_ceil(mip_h);
            mip_d = std::bit_ceil(mip_d);
        }

        size_t mip_size = 0;
        switch (tiling_mode) {
        case AmdGpu::TilingMode::Display_Linear: {
            ASSERT(!is_cube);
            mip_size = ImageSizeLinearAligned(mip_w, mip_h, bpp, num_samples);
            break;
        }
        case AmdGpu::TilingMode::Texture_MicroTiled: {
            mip_size = ImageSizeMicroTiled(mip_w, mip_h, bpp, num_samples);
            break;
        }
        case AmdGpu::TilingMode::Display_MacroTiled:
        case AmdGpu::TilingMode::Texture_MacroTiled:
        case AmdGpu::TilingMode::Depth_MacroTiled: {
            ASSERT(!is_cube && !is_block);
            ASSERT(num_samples == 1);
            ASSERT(num_bits <= 64);
            mip_size = ImageSizeMacroTiled(mip_w, mip_h, bpp, num_samples, image.tiling_index);
            break;
        }
        default: {
            UNREACHABLE();
        }
        }
        mip_size *= mip_d;

        mips_layout.emplace_back(guest_size_bytes, mip_size);
        guest_size_bytes += mip_size;
    }
    guest_size_bytes *= resources.layers;
}

} // namespace VideoCore
