#pragma once
#include "Core.hpp"
#include <cassert>

#ifdef RENDER_TOOLS_ERROR_CHECKS
#define RT_ERROR(x) x;
#else
#define RT_ERROR(x)
#endif

/*
Unfortunately this is quite complicated overall.
Theres not much that can be done to simplify it either, as the complexity is inherent to texture management.

TexFormat is a composite of TexComponent and TexType,
there is no composite for PixelComponent and PixelFormat 
*/
namespace rt {
    enum class TexComponent {
        R = 1 << 8,
        RG = 2 << 8,
        RGB = 3 << 8,
        RGBA = 4 << 8,
        Depth = 5 << 8,
        Stencil = 6 << 8,
        DepthStencil = 7 << 8,
        _Count = 7,
    };
    enum class TexType {
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
        R_N8 = (int)TexComponent::R | (int)TexType::N8,
        RG_N8 = (int)TexComponent::RG | (int)TexType::N8,
        RGB_N8 = (int)TexComponent::RGB | (int)TexType::N8,
        RGBA_N8 = (int)TexComponent::RGBA | (int)TexType::N8,

        R_N16 = (int)TexComponent::R | (int)TexType::N16,
        RG_N16 = (int)TexComponent::RG | (int)TexType::N16,
        RGB_N16 = (int)TexComponent::RGB | (int)TexType::N16,
        RGBA_N16 = (int)TexComponent::RGBA | (int)TexType::N16,

        // Normalized signed integer
        R_SN8 = (int)TexComponent::R | (int)TexType::SN8,
        RG_SN8 = (int)TexComponent::RG | (int)TexType::SN8,
        RGB_SN8 = (int)TexComponent::RGB | (int)TexType::SN8,
        RGBA_SN8 = (int)TexComponent::RGBA | (int)TexType::SN8,

        R_SN16 = (int)TexComponent::R | (int)TexType::SN16,
        RG_SN16 = (int)TexComponent::RG | (int)TexType::SN16,
        RGB_SN16 = (int)TexComponent::RGB | (int)TexType::SN16,
        RGBA_SN16 = (int)TexComponent::RGBA | (int)TexType::SN16,

        // Signed integer
        R_I8 = (int)TexComponent::R | (int)TexType::I8,
        RG_I8 = (int)TexComponent::RG | (int)TexType::I8,
        RGB_I8 = (int)TexComponent::RGB | (int)TexType::I8,
        RGBA_I8 = (int)TexComponent::RGBA | (int)TexType::I8,

        R_I16 = (int)TexComponent::R | (int)TexType::I16,
        RG_I16 = (int)TexComponent::RG | (int)TexType::I16,
        RGB_I16 = (int)TexComponent::RGB | (int)TexType::I16,
        RGBA_I16 = (int)TexComponent::RGBA | (int)TexType::I16,

        R_I32 = (int)TexComponent::R | (int)TexType::I32,
        RG_I32 = (int)TexComponent::RG | (int)TexType::I32,
        RGB_I32 = (int)TexComponent::RGB | (int)TexType::I32,
        RGBA_I32 = (int)TexComponent::RGBA | (int)TexType::I32,

        // Unsigned integer
        R_U8 = (int)TexComponent::R | (int)TexType::U8,
        RG_U8 = (int)TexComponent::RG | (int)TexType::U8,
        RGB_U8 = (int)TexComponent::RGB | (int)TexType::U8,
        RGBA_U8 = (int)TexComponent::RGBA | (int)TexType::U8,

        R_U16 = (int)TexComponent::R | (int)TexType::U16,
        RG_U16 = (int)TexComponent::RG | (int)TexType::U16,
        RGB_U16 = (int)TexComponent::RGB | (int)TexType::U16,
        RGBA_U16 = (int)TexComponent::RGBA | (int)TexType::U16,

        R_U32 = (int)TexComponent::R | (int)TexType::U32,
        RG_U32 = (int)TexComponent::RG | (int)TexType::U32,
        RGB_U32 = (int)TexComponent::RGB | (int)TexType::U32,
        RGBA_U32 = (int)TexComponent::RGBA | (int)TexType::U32,

        // Floating point
        R_F16 = (int)TexComponent::R | (int)TexType::F16,
        RG_F16 = (int)TexComponent::RG | (int)TexType::F16,
        RGB_F16 = (int)TexComponent::RGB | (int)TexType::F16,
        RGBA_F16 = (int)TexComponent::RGBA | (int)TexType::F16,

        R_F32 = (int)TexComponent::R | (int)TexType::F32,
        RG_F32 = (int)TexComponent::RG | (int)TexType::F32,
        RGB_F32 = (int)TexComponent::RGB | (int)TexType::F32,
        RGBA_F32 = (int)TexComponent::RGBA | (int)TexType::F32,

        // Depth
        D16 = (int)TexType::D16,
        D24 = (int)TexType::D24,
        D32 = (int)TexType::D32,

        // Depth Stencil
        D24_S8 = (int)TexType::D24_S8,
        D32_S8 = (int)TexType::D32_S8,

        // Stencil
        S8 = (int)TexType::S8,

        _Count = 54
    };

    enum class PixelComponent {
        R,
        RG,
        RGB,
        BGR,
        RGBA,
        BGRA,
        Stencil,
        Depth,
    };
    enum class PixelFormat {
        U8,
        U16,
        U24,
        U32,
        F16,
        F32,

        U8_3_3_2,
        U16_5_6_5,

        RU8_3_3_2,
        RU16_5_6_5,

        U16_4_4_4_4,
        RU16_4_4_4_4,
        U16_5_5_5_1,
        RU16_5_5_5_1,
        U32_8_8_8_8,
        RU32_8_8_8_8,
        U32_10_10_10_2,
        RU32_10_10_10_2,
    };

    static std::string_view to_string_view(TexComponent val) {
        switch (val) {
        case TexComponent::R:
            return intern::makeView("R");
        case TexComponent::RG:
            return intern::makeView("RG");
        case TexComponent::RGB:
            return intern::makeView("RGB");
        case TexComponent::RGBA:
            return intern::makeView("RGBA");
        case TexComponent::Depth:
            return intern::makeView("Depth");
        case TexComponent::Stencil:
            return intern::makeView("Stencil");
        case TexComponent::DepthStencil:
            return intern::makeView("DepthStencil");
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::to_string_view! Expected a TexComponent enumerator."));
        assert(false);
        return {};
    }
    static std::string_view to_string_view(TexType val) {
        switch (val) {
        case TexType::N8:
            return intern::makeView("N8");
        case TexType::N16:
            return intern::makeView("N16");
        case TexType::SN8:
            return intern::makeView("SN8");
        case TexType::SN16:
            return intern::makeView("SN16");
        case TexType::I8:
            return intern::makeView("I8");
        case TexType::I16:
            return intern::makeView("I16");
        case TexType::I32:
            return intern::makeView("I32");
        case TexType::U8:
            return intern::makeView("U8");
        case TexType::U16:
            return intern::makeView("U16");
        case TexType::U32:
            return intern::makeView("U32");
        case TexType::F16:
            return intern::makeView("F16");
        case TexType::F32:
            return intern::makeView("F32");
        case TexType::D16:
            return intern::makeView("D16");
        case TexType::D24:
            return intern::makeView("D24");
        case TexType::D32:
            return intern::makeView("D32");
        case TexType::D24_S8:
            return intern::makeView("D24_S8");
        case TexType::D32_S8:
            return intern::makeView("D32_S8");
        case TexType::S8:
            return intern::makeView("S8");
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::to_string_view! Expected a TexType enumerator."));
        assert(false);
        return {};
    }
    static std::string_view to_string_view(TexFormat val) {
        switch (val) {
        case TexFormat::R_N8:
            return intern::makeView("R_N8");
        case TexFormat::RG_N8:
	        return intern::makeView("RG_N8");
        case TexFormat::RGB_N8:
	        return intern::makeView("RGB_N8");
        case TexFormat::RGBA_N8:
	        return intern::makeView("RGBA_N8");
        case TexFormat::R_N16:
	        return intern::makeView("R_N16");
        case TexFormat::RG_N16:
	        return intern::makeView("RG_N16");
        case TexFormat::RGB_N16:
	            return intern::makeView("RGB_N16");
        case TexFormat::RGBA_N16:
	        return intern::makeView("RGBA_N16");
        case TexFormat::R_SN8:
	        return intern::makeView("R_SN8");
        case TexFormat::RG_SN8:
	        return intern::makeView("RG_SN8");
        case TexFormat::RGB_SN8:
	        return intern::makeView("RGB_SN8");
        case TexFormat::RGBA_SN8:
	        return intern::makeView("RGBA_SN8");
        case TexFormat::R_SN16:
	        return intern::makeView("R_SN16");
        case TexFormat::RG_SN16:
	        return intern::makeView("RG_SN16");
        case TexFormat::RGB_SN16:
	        return intern::makeView("RGB_SN16");
        case TexFormat::RGBA_SN16:
	        return intern::makeView("RGBA_SN16");
        case TexFormat::R_I8:
	        return intern::makeView("R_I8");
        case TexFormat::RG_I8:
	        return intern::makeView("RG_I8");
        case TexFormat::RGB_I8:
	        return intern::makeView("RGB_I8");
        case TexFormat::RGBA_I8:
	        return intern::makeView("RGBA_I8");
        case TexFormat::R_I16:
	        return intern::makeView("R_I16");
        case TexFormat::RG_I16:
	        return intern::makeView("RG_I16");
        case TexFormat::RGB_I16:
	        return intern::makeView("RGB_I16");
        case TexFormat::RGBA_I16:
	        return intern::makeView("RGBA_I16");
        case TexFormat::R_I32:
	        return intern::makeView("R_I32");
        case TexFormat::RG_I32:
	        return intern::makeView("RG_I32");
        case TexFormat::RGB_I32:
	        return intern::makeView("RGB_I32");
        case TexFormat::RGBA_I32:
	        return intern::makeView("RGBA_I32");
        case TexFormat::R_U8:
	        return intern::makeView("R_U8");
        case TexFormat::RG_U8:
	        return intern::makeView("RG_U8");
        case TexFormat::RGB_U8:
	        return intern::makeView("RGB_U8");
        case TexFormat::RGBA_U8:
	        return intern::makeView("RGBA_U8");
        case TexFormat::R_U16:
	        return intern::makeView("R_U16");
        case TexFormat::RG_U16:
	        return intern::makeView("RG_U16");
        case TexFormat::RGB_U16:
	        return intern::makeView("RGB_U16");
        case TexFormat::RGBA_U16:
	        return intern::makeView("RGBA_U16");
        case TexFormat::R_U32:
	        return intern::makeView("R_U32");
        case TexFormat::RG_U32:
	        return intern::makeView("RG_U32");
        case TexFormat::RGB_U32:
	        return intern::makeView("RGB_U32");
        case TexFormat::RGBA_U32:
	        return intern::makeView("RGBA_U32");
        case TexFormat::R_F16:
	        return intern::makeView("R_F16");
        case TexFormat::RG_F16:
	        return intern::makeView("RG_F16");
        case TexFormat::RGB_F16:
	        return intern::makeView("RGB_F16");
        case TexFormat::RGBA_F16:
	        return intern::makeView("RGBA_F16");
        case TexFormat::R_F32:
	        return intern::makeView("R_F32");
        case TexFormat::RG_F32:
	        return intern::makeView("RG_F32");
        case TexFormat::RGB_F32:
	        return intern::makeView("RGB_F32");
        case TexFormat::RGBA_F32:
	        return intern::makeView("RGBA_F32");
        case TexFormat::D16:
	        return intern::makeView("D16");
        case TexFormat::D24:
	        return intern::makeView("D24");
        case TexFormat::D32:
	        return intern::makeView("D32");
        case TexFormat::D24_S8:
	        return intern::makeView("D24_S8");
        case TexFormat::D32_S8:
	        return intern::makeView("D32_S8");
        case TexFormat::S8:
	        return intern::makeView("S8");
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::to_string_view! Expected a TexFormat enumerator."));
        assert(false);
        return {};
    }
    static std::string_view to_string_view(PixelComponent val) {
        switch (val) {
        case PixelComponent::R:
            return intern::makeView("R");
        case PixelComponent::RG:
            return intern::makeView("RG");
        case PixelComponent::RGB:
            return intern::makeView("RGB");
        case PixelComponent::BGR:
            return intern::makeView("BGR");
        case PixelComponent::RGBA:
            return intern::makeView("RGBA");
        case PixelComponent::BGRA:
            return intern::makeView("BGRA");
        case PixelComponent::Stencil:
            return intern::makeView("Stencil");
        case PixelComponent::Depth:
            return intern::makeView("Depth");
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::to_string_view! Expected a PixelComponent enumerator."));
        assert(false);
        return {};
    }
    static std::string_view to_string_view(PixelFormat val) {
        switch (val) {
        case PixelFormat::U8:
	    	return intern::makeView("U8");
        case PixelFormat::U16:
		    return intern::makeView("U16");
        case PixelFormat::U24:
		    return intern::makeView("U24");
        case PixelFormat::U32:
		    return intern::makeView("U32");
        case PixelFormat::F16:
		    return intern::makeView("F16");
        case PixelFormat::F32:
		    return intern::makeView("F32");
        case PixelFormat::U8_3_3_2:
		    return intern::makeView("U8_3_3_2");
        case PixelFormat::U16_5_6_5:
		    return intern::makeView("U16_5_6_5");
        case PixelFormat::RU8_3_3_2:
		    return intern::makeView("RU8_3_3_2");
        case PixelFormat::RU16_5_6_5:
		    return intern::makeView("RU16_5_6_5");
        case PixelFormat::U16_4_4_4_4:
		    return intern::makeView("U16_4_4_4_4");
        case PixelFormat::RU16_4_4_4_4:
		    return intern::makeView("RU16_4_4_4_4");
        case PixelFormat::U16_5_5_5_1:
		    return intern::makeView("U16_5_5_5_1");
        case PixelFormat::RU16_5_5_5_1:
		    return intern::makeView("RU16_5_5_5_1");
        case PixelFormat::U32_8_8_8_8:
		    return intern::makeView("U32_8_8_8_8");
        case PixelFormat::RU32_8_8_8_8:
		    return intern::makeView("RU32_8_8_8_8");
        case PixelFormat::U32_10_10_10_2:
		    return intern::makeView("U32_10_10_10_2");
        case PixelFormat::RU32_10_10_10_2:
		    return intern::makeView("RU32_10_10_10_2");
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::to_string_view! Expected a PixelFormat enumerator."));
        assert(false);
        return {};
    }


    static TexFormat combine(TexComponent comp, TexType size) noexcept {
        return static_cast<TexFormat>((int)comp | (int)size);
    }

    static TexType extractSize(TexFormat format) noexcept {
        return static_cast<TexType>((int)format & 0xFF);
    }

    static TexComponent extractComponent(TexFormat format) noexcept {
        return static_cast<TexComponent>((int)format & 0xFFFF00);
    }

    static bool isCompatible(TexComponent tcomp, PixelComponent pcomp) {
        switch (tcomp) {
        case TexComponent::R:
            return pcomp == PixelComponent::R;
        case TexComponent::RG:
            return pcomp == PixelComponent::RG;
        case TexComponent::RGB:
            return pcomp == PixelComponent::RGB || pcomp == PixelComponent::BGR;
        case TexComponent::RGBA:
            return pcomp == PixelComponent::RGBA || pcomp == PixelComponent::BGRA;
        case TexComponent::Depth:
            return pcomp == PixelComponent::Depth;
        case TexComponent::Stencil:
            return pcomp == PixelComponent::Stencil;
        case TexComponent::DepthStencil:
            return pcomp == PixelComponent::Stencil || pcomp == PixelComponent::Depth;
        }
        return false;
    }

    static constexpr GLenum convertGL(TexComponent comp) noexcept {
        switch (comp) {
        case TexComponent::R:
            return GL_RED;
        case TexComponent::RG:
            return GL_RG;
        case TexComponent::RGB:
            return GL_RGB;
        case TexComponent::RGBA:
            return GL_RGBA;
        case TexComponent::Depth:
            return GL_DEPTH_COMPONENT;
        case TexComponent::DepthStencil:
            return GL_DEPTH_STENCIL;
        case TexComponent::Stencil:
            return GL_STENCIL_INDEX;
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::convertGL! Expected a TexComponent enumerator."));
        assert(false);
        return GL_RED;
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
            RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::convertGL! Expected a TexFormat enumerator."));
            assert(false);
            return 0;
        }
    }

    // TODO: Test a bunch of these combinations.
    static GLenum convertGL(PixelComponent comp) {
        switch (comp) {
        case PixelComponent::R:
            return GL_RED;
        case PixelComponent::RG:
            return GL_RG;
        case PixelComponent::RGB:
            return GL_RGB;
        case PixelComponent::BGR:
            return GL_BGR;
        case PixelComponent::RGBA:
            return GL_RGBA;
        case PixelComponent::BGRA:
            return GL_BGRA;
        case PixelComponent::Depth:
            return GL_DEPTH_COMPONENT;
        case PixelComponent::Stencil:
            // No actual stencil index, has to be normal GL_RED?
            return GL_RED;
        }
        RT_ERROR(fmt::print(stderr, "Unexpected value passed into rt::convertGL! Expected a PixelComponent enumerator."));
        assert(false);
        return 0;
    }

    static GLenum convertGL(PixelComponent comp, PixelFormat format) {
        switch (comp) {
        case PixelComponent::R: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::RG: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::RGB: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            case PixelFormat::U8_3_3_2:
                return GL_UNSIGNED_BYTE_3_3_2;
            case PixelFormat::U16_5_6_5:
                return GL_UNSIGNED_SHORT_5_6_5;
            case PixelFormat::RU8_3_3_2:
                return GL_UNSIGNED_BYTE_2_3_3_REV;
            case PixelFormat::RU16_5_6_5:
                return GL_UNSIGNED_SHORT_5_6_5_REV;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::BGR: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            case PixelFormat::U8_3_3_2:
                return GL_UNSIGNED_BYTE_2_3_3_REV;
            case PixelFormat::U16_5_6_5:
                return GL_UNSIGNED_SHORT_5_6_5_REV;
            case PixelFormat::RU8_3_3_2:
                return GL_UNSIGNED_BYTE_3_3_2;
            case PixelFormat::RU16_5_6_5:
                return GL_UNSIGNED_SHORT_5_6_5;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::RGBA: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            case PixelFormat::U16_4_4_4_4:
                return  GL_UNSIGNED_SHORT_4_4_4_4;
            case PixelFormat::RU16_4_4_4_4:
                return GL_UNSIGNED_SHORT_4_4_4_4_REV;
            case PixelFormat::U16_5_5_5_1:
                return  GL_UNSIGNED_SHORT_5_5_5_1;
            case PixelFormat::RU16_5_5_5_1:
                return  GL_UNSIGNED_SHORT_1_5_5_5_REV;
            case PixelFormat::U32_8_8_8_8:
                return GL_UNSIGNED_INT_8_8_8_8;
            case PixelFormat::RU32_8_8_8_8:
                return  GL_UNSIGNED_INT_8_8_8_8_REV;
            case PixelFormat::U32_10_10_10_2:
                return  GL_UNSIGNED_INT_10_10_10_2;
            case PixelFormat::RU32_10_10_10_2:
                return GL_UNSIGNED_INT_2_10_10_10_REV;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::BGRA: {
            switch (format) {
            case PixelFormat::U8:
                return GL_UNSIGNED_BYTE;
            case PixelFormat::U16:
                return GL_UNSIGNED_SHORT;
            case PixelFormat::U32:
                return GL_UNSIGNED_INT;
            case PixelFormat::F16:
                return GL_HALF_FLOAT;
            case PixelFormat::F32:
                return GL_FLOAT;
            case PixelFormat::U16_4_4_4_4:
                return GL_UNSIGNED_SHORT_4_4_4_4;
            case PixelFormat::RU16_4_4_4_4:
                return GL_UNSIGNED_SHORT_4_4_4_4_REV;
            case PixelFormat::U16_5_5_5_1:
                return GL_UNSIGNED_SHORT_5_5_5_1;
            case PixelFormat::RU16_5_5_5_1:
                return GL_UNSIGNED_SHORT_1_5_5_5_REV;
            case PixelFormat::U32_8_8_8_8:
                return GL_UNSIGNED_INT_8_8_8_8;
            case PixelFormat::RU32_8_8_8_8:
                return GL_UNSIGNED_INT_8_8_8_8_REV;
            case PixelFormat::U32_10_10_10_2:
                return GL_UNSIGNED_INT_10_10_10_2;
            case PixelFormat::RU32_10_10_10_2:
                return GL_UNSIGNED_INT_2_10_10_10_REV;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::Depth: {
            switch (format) {
            case PixelFormat::U16:
                return GL_DEPTH_COMPONENT16;
            case PixelFormat::U24:
                return GL_DEPTH_COMPONENT24;
            case PixelFormat::U32:
                return GL_DEPTH_COMPONENT32;
            case PixelFormat::F32:
                return GL_DEPTH_COMPONENT32F;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }
        case PixelComponent::Stencil: {
            switch (format) {
            case PixelFormat::U8:
                return GL_STENCIL_INDEX8;
            case PixelFormat::U16:
                return GL_STENCIL_INDEX16;
            default:
                RT_ERROR(fmt::print(stderr, "Incompatible combination of PixelComponent and PixelFormat passed into rt::convertGL!"));
                RT_ERROR(fmt::print(stderr, "The values passed in were PixelComponent::{} and PixelFormat::{}\n", to_string_view(comp), to_string_view(format)));
                assert(false);
                return 0;
            }
        }}

        assert(false);
        return 0;
    }
};

#undef RT_ERROR