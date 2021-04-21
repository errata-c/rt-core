#pragma once
#include "TextureBase.hpp"

namespace rt {
    class Texture1dBase : public TextureBase {
    public:
        Texture1dBase(GLenum texType)
            : width(0)
        {
            glCreateTextures(texType, 1, &id);
            checkError();
        }

        Texture1dBase(const Texture1dBase&) = delete;
        Texture1dBase& operator=(const Texture1dBase&) = delete;

        Texture1dBase(Texture1dBase&& other) noexcept
            : TextureBase(static_cast<TextureBase&&>(other))
            , width(other.width)
        {}
        Texture1dBase& operator=(Texture1dBase&& other) noexcept {
            TextureBase::operator=(static_cast<TextureBase&&>(other));
            width = other.width;
        }

        bool isValidSize() const {
            return width > 0;
        }

        void invalidate(GLint level, GLint offset, GLint region) {
            assert(level >= 0);
            glInvalidateTexSubImage(id, level, offset, 0, 0, region, 1, 1);
            checkError();
        }

        void copyTo(Texture1dBase& other, GLint readLevel, GLint writeLevel, GLint read, GLint write, GLint region) {
            assert(readLevel >= 0);
            assert(writeLevel >= 0);
            assert(boundsCheck(read, region, readLevel));
            assert(other.boundsCheck(write, region, writeLevel));
            glCopyImageSubData(id, GL_TEXTURE_1D, readLevel, read, 0, 0, other.getId(), GL_TEXTURE_2D, writeLevel, write, 0, 0, region, 1, 1);
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
        void subImage(const void* data, GLint level, GLint offset, GLint region, PixelComponent comp, PixelFormat form) {
            assert(isCompatible(extractComponent(format), comp) && "Incompatible component type for texture!");
            subImage1D(data, level, offset, region, comp, form);
        }

        GLint getWidth() const {
            return width;
        }
        GLint getSize() const {
            return width;
        }

        bool boundsCheck(GLint xOffset, GLint regionWidth) const {
            return
                (regionWidth + xOffset) <= width;
        }
        bool boundsCheck(GLint xOffset, GLint regionWidth, GLint level) const {
            GLint subWidth = width >> level;
            return (xOffset + regionWidth) <= subWidth;
        }

        bool isInitialized() const noexcept {
            return width > 0;
        }
    private:
        GLint width;
    };
}