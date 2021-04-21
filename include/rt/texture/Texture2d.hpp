#pragma once
#include "TextureBase.hpp"

namespace rt {
    class Texture2dBase : public TextureBase {
    public:
        Texture2dBase(GLenum texType)
            : width(0)
            , height(0)
        {
            assert(texType == GL_TEXTURE_2D || texType == GL_TEXTURE_1D_ARRAY || texType == GL_TEXTURE_2D_MULTISAMPLE);
            glCreateTextures(texType, 1, &id);
            checkError();
        }
        Texture2dBase(GLenum texType, GLint w, GLint h)
            : width(w)
            , height(h)
        {
            assert(texType == GL_TEXTURE_2D || texType == GL_TEXTURE_1D_ARRAY || texType == GL_TEXTURE_2D_MULTISAMPLE);
            glCreateTextures(texType, 1, &id);
            checkError();
        }
        ~Texture2dBase() = default;

        Texture2dBase(const Texture2dBase&) = delete;
        Texture2dBase& operator=(const Texture2dBase&) = delete;

        Texture2dBase(Texture2dBase&& other) noexcept
            : TextureBase(static_cast<TextureBase&&>(other))
            , width(other.width)
            , height(other.height)
        {}
        Texture2dBase& operator=(Texture2dBase&& other) noexcept {
            TextureBase::operator=(static_cast<TextureBase&&>(other));
            width = other.width;
            height = other.height;
        }

        void invalidate(GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            assert(level >= 0);
            glInvalidateTexSubImage(id, level, offset.x, offset.y, 0, region.x, region.y, 1);
            checkError();
        }

        void copyTo(Texture2dBase& other, GLint readLevel, GLint writeLevel, const glm::ivec2& read, const glm::ivec2& write, const glm::ivec2& region) {
            assert(readLevel >= 0);
            assert(writeLevel >= 0);
            assert(boundsCheck(read, region, readLevel));
            assert(other.boundsCheck(write, region, writeLevel));
            glCopyImageSubData(id, GL_TEXTURE_2D, readLevel, read.x, read.y, 0, other.getId(), GL_TEXTURE_2D, writeLevel, write.x, write.y, 0, region.x, region.y, 1);
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
        void subImage(const void* data, GLint level, const glm::ivec2 offset, const glm::ivec2& region, PixelComponent comp, PixelFormat form) {
            assert(isCompatible(extractComponent(format), comp) && "Incompatible component type for texture!");
            subImage2D(data, level, offset, region, comp, form);
        }

        bool isValidSize() const {
            return width > 0 && height > 0;
        }

        GLint getWidth() const {
            return width;
        }
        GLint getHeight() const {
            return height;
        }
        glm::ivec2 getSize() const {
            return glm::ivec2(width, height);
        }

        bool boundsCheck(const glm::ivec2& offset, const glm::ivec2& region) const {
            return
                ((region.x + offset.x) <= width) &&
                ((region.y + offset.y) <= height);
        }
        bool boundsCheck(const glm::ivec2& offset, const glm::ivec2& region, GLint level) const {
            GLint subWidth = width >> level;
            GLint subHeight = height >> level;
            return
                ((region.x + offset.x) <= subWidth) &&
                ((region.y + offset.y) <= subHeight);
        }

        bool isInitialized() const noexcept {
            return width > 0 && height > 0;
        }
    protected:
        GLint width, height;
    };

    class ImmutableTexture2d : public Texture2dBase {
    public:
        ImmutableTexture2d()
            : Texture2dBase(GL_TEXTURE_2D)
        {}

        ImmutableTexture2d(const ImmutableTexture2d&) = delete;
        ImmutableTexture2d& operator=(const ImmutableTexture2d&) = delete;

        void init(TexFormat form, GLint mipLevels, const glm::ivec2& size) {
            assert(mipLevels != 0);
            assert(size.x > 0);
            assert(size.y > 0);

            format = form;

            //GLenum compEnum = convertGL(extractComponent(format));
            GLenum formEnum = convertGL(format);

            glTextureStorage2D(id, mipLevels, formEnum, size.x, size.y);
            checkError();
            width = size.x;
            height = size.y;
        }

        void reset() {
            if (isValid()) {
                glDeleteTextures(1, &id);
                checkError();

                glCreateTextures(GL_TEXTURE_2D, 1, &id);
                checkError();

                width = 0;
                height = 0;
            }
        }
    private:
    };
}