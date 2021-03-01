#pragma once
#include "Core.hpp"
#include <cassert>

namespace rt {
    enum class TexComponent {
        R = 1 << 8,
        RG = 2 << 8,
        RGB = 3 << 8,
        RGBA = 4 << 8,
        DEPTH = 5 << 8,
        STENCIL = 6 << 8,
        DEPTH_STENCIL = 7 << 8,
        _Count = 7,
    };

    enum class TexSize {
        // Normalized unsigned integer
        N8,
        N16,

        // Normalized signed integer
        SN8,
        SN16,

        // Signed integer
        I8,
        I16,
        I32,

        // Unsigned integer
        U8,
        U16,
        U32,

        // Floating point
        F16,
        F32,

        // Depth
        D16,
        D24,
        D32,

        // Depth Stencil
        D24_S8,
        D32_S8,

        // Stencil
        S8,

        _Count
    };

    enum class TexFormat {
        // Normalized unsigned integer
        R_N8 = (int)TexComponent::R | (int)TexSize::N8,
        RG_N8 = (int)TexComponent::RG | (int)TexSize::N8,
        RGB_N8 = (int)TexComponent::RGB | (int)TexSize::N8,
        RGBA_N8 = (int)TexComponent::RGBA | (int)TexSize::N8,

        R_N16 = (int)TexComponent::R | (int)TexSize::N16,
        RG_N16 = (int)TexComponent::RG | (int)TexSize::N16,
        RGB_N16 = (int)TexComponent::RGB | (int)TexSize::N16,
        RGBA_N16 = (int)TexComponent::RGBA | (int)TexSize::N16,

        // Normalized signed integer
        R_SN8 = (int)TexComponent::R | (int)TexSize::SN8,
        RG_SN8 = (int)TexComponent::RG | (int)TexSize::SN8,
        RGB_SN8 = (int)TexComponent::RGB | (int)TexSize::SN8,
        RGBA_SN8 = (int)TexComponent::RGBA | (int)TexSize::SN8,

        R_SN16 = (int)TexComponent::R | (int)TexSize::SN16,
        RG_SN16 = (int)TexComponent::RG | (int)TexSize::SN16,
        RGB_SN16 = (int)TexComponent::RGB | (int)TexSize::SN16,
        RGBA_SN16 = (int)TexComponent::RGBA | (int)TexSize::SN16,

        // Signed integer
        R_I8 = (int)TexComponent::R | (int)TexSize::I8,
        RG_I8 = (int)TexComponent::RG | (int)TexSize::I8,
        RGB_I8 = (int)TexComponent::RGB | (int)TexSize::I8,
        RGBA_I8 = (int)TexComponent::RGBA | (int)TexSize::I8,

        R_I16 = (int)TexComponent::R | (int)TexSize::I16,
        RG_I16 = (int)TexComponent::RG | (int)TexSize::I16,
        RGB_I16 = (int)TexComponent::RGB | (int)TexSize::I16,
        RGBA_I16 = (int)TexComponent::RGBA | (int)TexSize::I16,

        R_I32 = (int)TexComponent::R | (int)TexSize::I32,
        RG_I32 = (int)TexComponent::RG | (int)TexSize::I32,
        RGB_I32 = (int)TexComponent::RGB | (int)TexSize::I32,
        RGBA_I32 = (int)TexComponent::RGBA | (int)TexSize::I32,

        // Unsigned integer
        R_U8 = (int)TexComponent::R | (int)TexSize::U8,
        RG_U8 = (int)TexComponent::RG | (int)TexSize::U8,
        RGB_U8 = (int)TexComponent::RGB | (int)TexSize::U8,
        RGBA_U8 = (int)TexComponent::RGBA | (int)TexSize::U8,

        R_U16 = (int)TexComponent::R | (int)TexSize::U16,
        RG_U16 = (int)TexComponent::RG | (int)TexSize::U16,
        RGB_U16 = (int)TexComponent::RGB | (int)TexSize::U16,
        RGBA_U16 = (int)TexComponent::RGBA | (int)TexSize::U16,

        R_U32 = (int)TexComponent::R | (int)TexSize::U32,
        RG_U32 = (int)TexComponent::RG | (int)TexSize::U32,
        RGB_U32 = (int)TexComponent::RGB | (int)TexSize::U32,
        RGBA_U32 = (int)TexComponent::RGBA | (int)TexSize::U32,

        // Floating point
        R_F16 = (int)TexComponent::R | (int)TexSize::F16,
        RG_F16 = (int)TexComponent::RG | (int)TexSize::F16,
        RGB_F16 = (int)TexComponent::RGB | (int)TexSize::F16,
        RGBA_F16 = (int)TexComponent::RGBA | (int)TexSize::F16,

        R_F32 = (int)TexComponent::R | (int)TexSize::F32,
        RG_F32 = (int)TexComponent::RG | (int)TexSize::F32,
        RGB_F32 = (int)TexComponent::RGB | (int)TexSize::F32,
        RGBA_F32 = (int)TexComponent::RGBA | (int)TexSize::F32,

        // Depth
        D16 = (int)TexSize::D16,
        D24 = (int)TexSize::D24,
        D32 = (int)TexSize::D32,

        // Depth Stencil
        D24_S8 = (int)TexSize::D24_S8,
        D32_S8 = (int)TexSize::D32_S8,

        // Stencil
        S8 = (int)TexSize::S8,

        Count
    };

    static TexSize extractSize(TexFormat format) noexcept {
        return static_cast<TexSize>((int)format & 0xFFFF);
    }

    static TexComponent extractComponent(TexFormat format) noexcept {
        return static_cast<TexComponent>((int)format & 0xFFFF0000);
    }

    static constexpr GLenum convertGL(TexComponent comp) noexcept {
        switch (comp) {
        case TexComponent::R:
            return GL_R;
        case TexComponent::RG:
            return GL_RG;
        case TexComponent::RGB:
            return GL_RGB;
        case TexComponent::RGBA:
            return GL_RGBA;
        case TexComponent::DEPTH:
            return GL_DEPTH;
        case TexComponent::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL;
        case TexComponent::STENCIL:
            return GL_STENCIL;
        }
        return GL_R;
    }

    static constexpr GLenum convertGL(TexFormat format) noexcept {
        switch (format) {
        case TexFormat::R_F16:
            return GL_R16F;
        case TexFormat::R_F32:
            return GL_R32F;
        case TexFormat::R_N8:
            return GL_R8;
        case TexFormat::R_N16:
            return GL_R16;
        case TexFormat::R_I8:
            return GL_R8I;
        case TexFormat::R_I16:
            return GL_R16I;
        case TexFormat::R_I32:
            return GL_R32I;
        case TexFormat::R_U8:
            return GL_R8UI;
        case TexFormat::R_U16:
            return GL_R16UI;
        case TexFormat::R_U32:
            return GL_R32UI;
        case TexFormat::R_SN8:
            return GL_R8_SNORM;
        case TexFormat::R_SN16:
            return GL_R16_SNORM;

        case TexFormat::RG_F16:
            return GL_RG16F;
        case TexFormat::RG_F32:
            return GL_RG32F;
        case TexFormat::RG_N8:
            return GL_RG8;
        case TexFormat::RG_N16:
            return GL_RG16;
        case TexFormat::RG_SN8:
            return GL_RG8_SNORM;
        case TexFormat::RG_SN16:
            return GL_RG16_SNORM;
        case TexFormat::RG_I8:
            return GL_RG8I;
        case TexFormat::RG_I16:
            return GL_RG16I;
        case TexFormat::RG_I32:
            return GL_RG32I;
        case TexFormat::RG_U8:
            return GL_RG8UI;
        case TexFormat::RG_U16:
            return GL_RG16UI;
        case TexFormat::RG_U32:
            return GL_RG32UI;
        

        case TexFormat::RGB_F16:
            return GL_RGB16F;
        case TexFormat::RGB_F32:
            return GL_RGB32F;
        case TexFormat::RGB_N8:
            return GL_RGB8;
        case TexFormat::RGB_N16:
            return GL_RGB16;
        case TexFormat::RGB_I8:
            return GL_RGB8I;
        case TexFormat::RGB_I16:
            return GL_RGB16I;
        case TexFormat::RGB_I32:
            return GL_RGB32I;
        case TexFormat::RGB_U8:
            return GL_RGB8UI;
        case TexFormat::RGB_U16:
            return GL_RGB16UI;
        case TexFormat::RGB_U32:
            return GL_RGB32UI;
        case TexFormat::RGB_SN8:
            return GL_RGB8_SNORM;
        case TexFormat::RGB_SN16:
            return GL_RGB16_SNORM;


        case TexFormat::RGBA_F16:
            return GL_RGBA16F;
        case TexFormat::RGBA_F32:
            return GL_RGBA32F;
        case TexFormat::RGBA_N8:
            return GL_RGBA8;
        case TexFormat::RGBA_N16:
            return GL_RGBA16;
        case TexFormat::RGBA_SN8:
            return GL_RGBA8_SNORM;
        case TexFormat::RGBA_SN16:
            return GL_RGBA16_SNORM;
        case TexFormat::RGBA_I8:
            return GL_RGBA8I;
        case TexFormat::RGBA_I16:
            return GL_RGBA16I;
        case TexFormat::RGBA_I32:
            return GL_RGBA32I;
        case TexFormat::RGBA_U8:
            return GL_RGBA8UI;
        case TexFormat::RGBA_U16:
            return GL_RGBA16UI;
        case TexFormat::RGBA_U32:
            return GL_RGBA32UI;
            
        case TexFormat::D16:
            return GL_DEPTH_COMPONENT16;
        case TexFormat::D24:
            return GL_DEPTH_COMPONENT24;
        case TexFormat::D32:
            return GL_DEPTH_COMPONENT32;
        case TexFormat::D24_S8:
            return GL_DEPTH24_STENCIL8;
        case TexFormat::D32_S8:
            return GL_DEPTH32F_STENCIL8;
        case TexFormat::S8:
            return GL_STENCIL_INDEX8;

        default:
            return 0;
        }
    }
};