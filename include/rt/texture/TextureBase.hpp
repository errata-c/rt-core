#pragma once
#include "../TextureUtilities.hpp"
#include <cassert>
#include <array>

#ifdef RENDER_TOOLS_COMPREHENSIVE_ERROR_CHECKS
#define RT_ERROR(x) x
#else 
#define RT_ERROR(x)
#endif

namespace rt {
    class TextureBase {
    public:
        TextureBase()
            : id(0)
            , texWrap(GL_REPEAT)
            , minFilter(GL_NEAREST_MIPMAP_LINEAR)
            , magFilter(GL_LINEAR)
            , format(TexFormat::R_N8)
        {}
        TextureBase(TextureBase&& other) noexcept
            : id(other.id)
            , texWrap(other.texWrap)
            , minFilter(other.minFilter)
            , magFilter(other.magFilter)
            , format(other.format)
        {
            other.id = 0;
        }
        ~TextureBase() {
            if (isValid()) {
                glDeleteTextures(1, &id);
                id = 0;
            }
        }

        TextureBase(const TextureBase&) = delete;
        TextureBase& operator=(const TextureBase&) = delete;

        TextureBase& operator=(TextureBase&& other) noexcept {
            if (isValid()) {
                glDeleteTextures(1, &id);
                checkError();
            }
            id = other.id;
            minFilter = other.minFilter;
            magFilter = other.magFilter;
            texWrap = other.texWrap;
            format = other.format;

            other.id = 0;

            return *this;
        }

        void setBorderColor(float r, float g, float b, float a) {
            setBorderColor(glm::vec4{ r, g, b, a });
        }
        void setBorderColor(glm::vec4 color) {
            glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
            checkError();
        }

        glm::vec4 getBorderColor() const {
            glm::vec4 arr;
            glGetTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(arr));
            checkError();
            return arr;
        }

        void setSwizzle(GLint r, GLint g, GLint b, GLint a) {
            assert(r >= 0 && r < 4);
            assert(g >= 0 && g < 4);
            assert(b >= 0 && b < 4);
            assert(a >= 0 && a < 4);
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &r); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &g); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_B, &b); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_A, &a); checkError();
        }
        void setSwizzle(GLint r, GLint g, GLint b) {
            assert(r >= 0 && r < 4);
            assert(g >= 0 && g < 4);
            assert(b >= 0 && b < 4);
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &r); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &g); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_B, &b); checkError();
        }
        void setSwizzle(GLint r, GLint g) {
            assert(r >= 0 && r < 4);
            assert(g >= 0 && g < 4);
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &r); checkError();
            glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &g); checkError();
        }

        GLint getRedSwizzle() const {
            GLint index = 0;
            glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &index);
            checkError();
            return index - GL_TEXTURE_SWIZZLE_R;
        }
        GLint getGreenSwizzle() const {
            GLint index = 0;
            glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &index);
            checkError();
            return index - GL_TEXTURE_SWIZZLE_R;
        }
        GLint getBlueSwizzle() const {
            GLint index = 0;
            glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_B, &index);
            checkError();
            return index - GL_TEXTURE_SWIZZLE_R;
        }
        GLint getAlphaSwizzle() const {
            GLint index = 0;
            glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_A, &index);
            checkError();
            return index - GL_TEXTURE_SWIZZLE_R;
        }

        glm::ivec4 getSwizzle() const {
            return glm::ivec4{ getRedSwizzle(), getGreenSwizzle(), getBlueSwizzle(), getAlphaSwizzle() };
        }

        bool isValid() const noexcept {
            return id != 0;
        }

        void bindUnit(GLuint texUnit) {
            glBindTextureUnit(texUnit, id);
            checkError();
        }

        void generateMipmaps() {
            glGenerateTextureMipmap(id);
            checkError();

            switch (minFilter) {
            case GL_LINEAR:
                minFilter = GL_NEAREST_MIPMAP_LINEAR;
                break;
            case GL_NEAREST:
                minFilter = GL_NEAREST_MIPMAP_LINEAR;
                break;
            }

            glTextureParameterIuiv(id, GL_TEXTURE_MIN_FILTER, &minFilter);
            checkError();
        }

        void invalidate(GLint level) {
            glInvalidateTexImage(id, level);
            checkError();
        }

        void filterLinear() {
            magFilter = GL_LINEAR;
            switch (minFilter) {
            case GL_LINEAR_MIPMAP_LINEAR:
                break;
            case GL_NEAREST_MIPMAP_LINEAR:
                minFilter = GL_LINEAR_MIPMAP_LINEAR;
                break;
            default:
                minFilter = GL_LINEAR;
                break;
            }
            glTextureParameterIuiv(id, GL_TEXTURE_MIN_FILTER, &minFilter);
            checkError();
            glTextureParameterIuiv(id, GL_TEXTURE_MAG_FILTER, &magFilter);
            checkError();
        }
        void filterNearest() {
            magFilter = GL_NEAREST;
            switch (minFilter) {
            case GL_LINEAR_MIPMAP_LINEAR:
                minFilter = GL_NEAREST_MIPMAP_LINEAR;
                break;
            case GL_NEAREST_MIPMAP_LINEAR:
                break;
            default:
                minFilter = GL_NEAREST;
                break;
            }
            glTextureParameterIuiv(id, GL_TEXTURE_MIN_FILTER, &minFilter);
            checkError();
            glTextureParameterIuiv(id, GL_TEXTURE_MAG_FILTER, &magFilter);
            checkError();
        }

        void setBorderColor(float r, float g, float b) {
            setBorderColor(glm::vec4(r, g, b, 1.f));
        }
        void setBorderColor(glm::vec3 color) {
            setBorderColor(glm::vec4(color, 1.f));
        }

        void clampEdge() {
            texWrap = GL_CLAMP_TO_EDGE;
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); checkError();
        }
        void clampBorder() {
            texWrap = GL_CLAMP_TO_BORDER;
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER); checkError();
        }
        void makeRepeat() {
            texWrap = GL_REPEAT;
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_REPEAT); checkError();
        }
        void makeMirroredRepeat() {
            texWrap = GL_MIRRORED_REPEAT;
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); checkError();
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT); checkError();
        }

        bool isRepeating() const noexcept {
            return texWrap == GL_REPEAT;
        }
        bool isClampedToEdge() const noexcept {
            return texWrap == GL_CLAMP_TO_EDGE;
        }
        bool isClampedToBorder() const noexcept {
            return texWrap == GL_CLAMP_TO_BORDER;
        }
        bool isMirroredRepeating() const noexcept {
            return texWrap == GL_MIRRORED_REPEAT;
        }

        bool isFilterLinear() const noexcept {
            return magFilter == GL_LINEAR;
        }
        bool isFilterNearest() const noexcept {
            return magFilter == GL_NEAREST;
        }

        GLenum getMagFilter() const noexcept {
            return magFilter;
        }

        GLenum getMinFilter() const noexcept {
            return minFilter;
        }

        GLuint getId() const noexcept {
            return id;
        }

        GLenum getGLTarget() const {
            GLint value = 0;
            glGetTextureParameteriv(id, GL_TEXTURE_TARGET, &value);
            checkError();
            return *reinterpret_cast<GLenum*>(&value);
        }

        /// <summary>
        /// Substitute data into a texture object.
        /// </summary>
        /// <param name="data">The data to read from</param>
        /// <param name="level">The mipmap level to write to</param>
        /// <param name="offset">The offset into the texture to write to</param>
        /// <param name="region">The region to write to in the texture, this combined with the pixel formatting defines how much data will be read</param>
        /// <param name="comp">The component type of the data being read, this must be compatible with the component type of the texture</param>
        /// <param name="form">The format of the pixel data being read</param>
        void subImage1D(const void* data, GLint level, GLint offset, GLint region, PixelComponent comp, PixelFormat form) {
            assert(level >= 0);
            glTextureSubImage1D(getId(), level, offset, region, convertGL(comp), convertGL(comp, form), data);
            checkError();
        }

        /// <summary>
        /// Substitute data into a texture object.
        /// </summary>
        /// <param name="data">The data to read from</param>
        /// <param name="level">The mipmap level to write to</param>
        /// <param name="offset">The offset into the texture to write to</param>
        /// <param name="region">The region to write to in the texture, this combined with the pixel formatting defines how much data will be read</param>
        /// <param name="comp">The component type of the data being read, this must be compatible with the component type of the texture</param>
        /// <param name="form">The format of the pixel data being read</param>
        void subImage2D(const void* data, GLint level, const glm::ivec2& offset, const glm::ivec2& region, PixelComponent comp, PixelFormat form) {
            assert(level >= 0);
            glTextureSubImage2D(getId(), level, offset.x, offset.y, region.x, region.y, convertGL(comp), convertGL(comp, form), data);
        }

        /// <summary>
        /// Substitute data into a texture object.
        /// </summary>
        /// <param name="data">The data to read from</param>
        /// <param name="level">The mipmap level to write to</param>
        /// <param name="offset">The offset into the texture to write to</param>
        /// <param name="region">The region to write to in the texture, this combined with the pixel formatting defines how much data will be read</param>
        /// <param name="comp">The component type of the data being read, this must be compatible with the component type of the texture</param>
        /// <param name="form">The format of the pixel data being read</param>
        void subImage3D(const void* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region, PixelComponent comp, PixelFormat form) {
            assert(level >= 0);
            glTextureSubImage3D(getId(), level, offset.x, offset.y, offset.z, region.x, region.y, region.z, convertGL(comp), convertGL(comp, form), data);
        }
    protected:
        void enableMipmapFilters() {
            if (minFilter != GL_LINEAR_MIPMAP_LINEAR) {
                minFilter = GL_LINEAR_MIPMAP_LINEAR;
                glTextureParameterIuiv(id, GL_TEXTURE_MIN_FILTER, &minFilter);
                checkError();
            }
        }
        void disableMipmapFilters() {
            if (minFilter == GL_LINEAR_MIPMAP_LINEAR) {
                minFilter = GL_LINEAR;
                glTextureParameterIuiv(id, GL_TEXTURE_MIN_FILTER, &minFilter);
                checkError();
            }
        }

        GLuint id;
        GLenum texWrap, minFilter, magFilter;
        TexFormat format;
    };
}

#undef RT_ERROR