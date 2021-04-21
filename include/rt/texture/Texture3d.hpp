#pragma once
#include "TextureBase.hpp"

namespace rt {
    class Texture3dBase : public TextureBase {
    public:
        Texture3dBase(GLenum texType)
            : width(0)
            , height(0)
            , depth(0)
        {
            assert(texType == GL_TEXTURE_3D || texType == GL_TEXTURE_2D_ARRAY);
            glCreateTextures(texType, 1, &id);
            checkError();
        }
        ~Texture3dBase() = default;

        Texture3dBase(const Texture3dBase&) = delete;
        Texture3dBase& operator=(const Texture3dBase&) = delete;

        Texture3dBase(Texture3dBase&& other) noexcept
            : TextureBase(static_cast<TextureBase&&>(other))
            , width(other.width)
            , height(other.height)
            , depth(other.depth)
        {}
        Texture3dBase& operator=(Texture3dBase&& other) noexcept {
            TextureBase::operator=(static_cast<TextureBase&&>(other));
            width = other.width;
            height = other.height;
            depth = other.depth;
        }

        void invalidate(GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            glInvalidateTexSubImage(id, level, offset.x, offset.y, offset.z, region.x, region.y, region.z);
            checkError();
        }

        /// <summary>
        /// Copy data from this texture object into another. The other object must be a 3d texture.
        /// </summary>
        /// <param name="other"></param>
        /// <param name="readLevel"></param>
        /// <param name="writeLevel"></param>
        /// <param name="read"></param>
        /// <param name="write"></param>
        /// <param name="region"></param>
        void copyTo(Texture3dBase& other, GLint readLevel, GLint writeLevel, glm::ivec3 read, glm::ivec3 write, glm::ivec3 region) {
            assert(boundsCheck(read, region, readLevel));
            assert(other.boundsCheck(write, region, writeLevel));
            glCopyImageSubData(id, GL_TEXTURE_3D, readLevel, read.x, read.y, read.z, other.getId(), GL_TEXTURE_3D, writeLevel, write.x, write.y, write.z, region.x, region.y, region.z);
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
        void subImage(const void* data, GLint level, const glm::ivec3 offset, const glm::ivec3& region, PixelComponent comp, PixelFormat form) {
            assert(isCompatible(extractComponent(format), comp) && "Incompatible component type for texture!");
            subImage3D(data, level, offset, region, comp, form);
        }

        bool isValidSize() const {
            return width > 0 && height > 0 && depth > 0;
        }

        GLint getWidth() const {
            return width;
        }
        GLint getHeight() const {
            return height;
        }
        GLint getDepth() const {
            return depth;
        }
        glm::ivec3 getSize() const {
            return glm::ivec3{ width, height, depth };
        }

        bool boundsCheck(const glm::ivec3& offset, const glm::ivec3& region) const {
            return
                ((region.x + offset.x) <= width) &&
                ((region.y + offset.y) <= height) &&
                ((region.z + offset.z) <= depth);
        }
        bool boundsCheck(const glm::ivec3& offset, const glm::ivec3& region, GLint level) const {
            GLint subWidth = width >> level;
            GLint subHeight = height >> level;
            GLint subDepth = depth >> level;

            return
                ((region.x + offset.x) <= subWidth) &&
                ((region.y + offset.y) <= subHeight) &&
                ((region.z + offset.z) <= subDepth);
        }

        bool isInitialized() const noexcept {
            return width > 0 && height > 0 && depth > 0;
        }
    protected:
        GLint width, height, depth;
    };

    class Texture2DArray : public Texture3dBase {
    public:
        Texture2DArray()
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {}

        Texture2DArray(TexFormat form, GLint mipLevels, const glm::ivec2& size, GLint numLayers)
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {
            init(form, mipLevels, glm::ivec3{ size, numLayers });
        }
        Texture2DArray(TexFormat form, GLint mipLevels, const glm::ivec3& size)
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {
            init(form, mipLevels, size);
        }

        Texture2DArray(const Texture2DArray&) = delete;
        Texture2DArray& operator=(const Texture2DArray&) = delete;

        void init(TexFormat form, GLint mipLevels, const glm::ivec2& size, GLint numLayers) {
            init(form, mipLevels, size, numLayers);
        }
        void init(TexFormat form, GLint mipLevels, const glm::ivec3& size) {
            assert(mipLevels > 0);
            assert(size.x > 0);
            assert(size.y > 0);
            assert(size.z > 0);

            format = form;
            GLenum formEnum = convertGL(format);

            glTextureStorage3D(id, mipLevels, formEnum, size.x, size.y, size.z); checkError();
            width = size.x;
            height = size.y;
            depth = size.z;
        }

        void reset() {
            if (isValid()) {
                glDeleteTextures(1, &id);
                checkError();

                glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);
                checkError();

                width = 0;
                height = 0;
                depth = 0;
            }
        }

        bool isInitialized() const noexcept {
            return width <= 0 && height <= 0 && depth <= 0;
        }
    private:
    };

    class ImmutableTexture3d : public Texture3dBase {
    public:
        ImmutableTexture3d()
            : Texture3dBase(GL_TEXTURE_3D)
        {}

        ImmutableTexture3d(TexComponent comp, TexFormat form, GLuint levels, const glm::ivec3& size)
            : Texture3dBase(GL_TEXTURE_3D)
        {
            init(comp, form, levels, size);
        }

        ImmutableTexture3d(const ImmutableTexture3d&) = delete;
        ImmutableTexture3d& operator=(const ImmutableTexture3d&) = delete;

        void init(TexComponent comp, TexFormat form, GLint mipLevels, const glm::ivec3& size) {
            assert(mipLevels > 0);
            assert(size.x > 0);
            assert(size.y > 0);
            assert(size.z > 0);

            format = form;
            GLenum formEnum = convertGL(format);

            glTextureStorage3D(id, mipLevels, formEnum, size.x, size.y, size.z);
            checkError();

            width = size.x;
            height = size.y;
            depth = size.z;
        }

        void reset() {
            if (isValid()) {
                glDeleteTextures(1, &id);
                checkError();

                glCreateTextures(GL_TEXTURE_3D, 1, &id);
                checkError();

                width = 0;
                height = 0;
                depth = 0;
            }
        }
    private:
    };
}