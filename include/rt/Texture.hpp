#pragma once
#include "TextureUtilities.hpp"
#include <cassert>
#include <array>

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
            setBorderColor(glm::vec4{r, g, b, a});
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
            return glm::ivec4{getRedSwizzle(), getGreenSwizzle(), getBlueSwizzle(), getAlphaSwizzle()};
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

    class Texture3dBase: public TextureBase {
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

        void copyTo(Texture3dBase& other, GLint readLevel, GLint writeLevel, glm::ivec3 read, glm::ivec3 write, glm::ivec3 region) {
            assert(boundsCheck(read, region, readLevel));
            assert(other.boundsCheck(write, region, writeLevel));
            glCopyImageSubData(id, GL_TEXTURE_3D, readLevel, read.x, read.y, read.z, other.getId(), GL_TEXTURE_3D, writeLevel, write.x, write.y, write.z, region.x, region.y, region.z);
            checkError();
        }

        void invalidate(GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            glInvalidateTexSubImage(id, level, offset.x, offset.y, 0, region.x, region.y, 1);
            checkError();
        }

    private:
        template<typename T>
        void _subImage(const T* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region, GLenum gltype) {
            assert(boundsCheck(offset, region, level));
            glTextureSubImage3D(id, level, offset.x, offset.y, offset.z, region.x, region.y, region.z, convertGL(format), gltype, (const void*)data);
            checkError();
        }
    public:
        void subImageI8(const GLbyte* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_BYTE);
        }
        void subImageI16(const GLshort* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_SHORT);
        }
        void subImageI32(const GLint* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_INT);
        }
        void subImageU8(const GLubyte* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_BYTE);
        }
        void subImageU16(const GLushort* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_SHORT);
        }
        void subImageU32(const GLuint* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT);
        }
        void subImageF32(const GLfloat* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_FLOAT);
        }
        void subImageU8_8_8_8(const GLuint* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8);
        }
        void subImageU8_8_8_8_rev(const GLuint* data, GLint level, const glm::ivec3& offset, const glm::ivec3& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8_REV);            
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
            return glm::ivec3{width, height, depth};
        }
        
        bool boundsCheck(const glm::ivec3 & offset, const glm::ivec3 & region) const {
            return 
                ((region.x + offset.x) <= width) && 
                ((region.y + offset.y) <= height) &&
                ((region.z + offset.z) <= depth);
        }
        bool boundsCheck(const glm::ivec3 & offset, const glm::ivec3 & region, GLint level) const {
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

    class Texture2dBase: public TextureBase {
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
        
        void invalidate(GLint level, const glm::ivec2 & offset, const glm::ivec2 & region) {
            assert(level >= 0);
            glInvalidateTexSubImage(id, level, offset.x, offset.y, 0, region.x, region.y, 1);
            checkError();
        }

        void copyTo(Texture2dBase & other, GLint readLevel, GLint writeLevel, const glm::ivec2 & read, const glm::ivec2 & write, const glm::ivec2& region) {
            assert(readLevel >= 0);
            assert(writeLevel >= 0);
            assert(boundsCheck(read, region, readLevel));
            assert(other.boundsCheck(write, region, writeLevel));
            glCopyImageSubData(id, GL_TEXTURE_2D, readLevel, read.x, read.y, 0, other.getId(), GL_TEXTURE_2D, writeLevel, write.x, write.y, 0, region.x, region.y, 1);
            checkError();
        }
        
    private:
        template<typename T>
        void _subImage(const T* data, GLint level, const glm::ivec2& offset, const glm::ivec2& region, GLenum gltype) {
            assert(boundsCheck(offset, region, level));
            glTextureSubImage2D(id, level, offset.x, offset.y, region.x, region.y, convertGL(format), gltype, (const void*)data);
            checkError();
        }
    public:

        void subImageI8(const GLubyte * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_BYTE);
        }
        void subImageI16(const GLushort * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_SHORT);
        }
        void subImageI32(const GLuint * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_INT);
        }
        void subImageU8(const GLubyte * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_BYTE);
        }
        void subImageU16(const GLushort * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_SHORT);
        }
        void subImageU32(const GLuint * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT);
        }
        void subImageF32(const GLfloat * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_FLOAT);
        }
        void subImageU8_8_8_8(const GLuint * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8);
        }
        void subImageU8_8_8_8_rev(const GLuint * data, GLint level, const glm::ivec2& offset, const glm::ivec2& region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8_REV);
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

        bool boundsCheck(const glm::ivec2 & offset, const glm::ivec2 & region) const {
            return 
                ((region.x + offset.x) <= width) &&
                ((region.y + offset.y) <= height);
        }
        bool boundsCheck(const glm::ivec2 & offset, const glm::ivec2 & region, GLint level) const {
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

    private:
        template<typename T>
        void _subImage(const T* data, GLint level, GLint offset, GLint region, GLenum gltype) {
            assert(boundsCheck(offset, region, level));
            glTextureSubImage1D(id, level, offset, region, convertGL(format), gltype, (const void*)data);
            checkError();
        }
    public:

        void subImageI8(const GLubyte* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_BYTE);
        }
        void subImageI16(const GLushort* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_SHORT);
        }
        void subImageI32(const GLuint* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_INT);
        }
        void subImageU8(const GLubyte* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_BYTE);
        }
        void subImageU16(const GLushort* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_SHORT);
        }
        void subImageU32(const GLuint* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT);
        }
        void subImageF32(const GLfloat* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_FLOAT);
        }
        void subImageU8_8_8_8(const GLuint* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8);
        }
        void subImageU8_8_8_8_rev(const GLuint* data, GLint level, GLint offset, GLint region) {
            _subImage(data, level, offset, region, GL_UNSIGNED_INT_8_8_8_8_REV);
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

    class ImmutableTexture2d : public Texture2dBase {
    public:
        ImmutableTexture2d()
            : Texture2dBase(GL_TEXTURE_2D)
        {}

        ImmutableTexture2d(const ImmutableTexture2d &) = delete;
        ImmutableTexture2d& operator=(const ImmutableTexture2d&) = delete;

        void init(TexFormat form, GLint mipLevels, const glm::ivec2 & size) {
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

    class Texture2DArray: public Texture3dBase {
    public:
        Texture2DArray()
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {}

        Texture2DArray(TexFormat form, GLint mipLevels, const glm::ivec2 & size, GLint numLayers)
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {
            init(form, mipLevels, glm::ivec3{size, numLayers});
        }
        Texture2DArray(TexFormat form, GLint mipLevels, const glm::ivec3 & size)
            : Texture3dBase(GL_TEXTURE_2D_ARRAY)
        {
            init(form, mipLevels, size);
        }

        Texture2DArray(const Texture2DArray&) = delete;
        Texture2DArray& operator=(const Texture2DArray&) = delete;

        void init(TexFormat form, GLint mipLevels, const glm::ivec2& size, GLint numLayers) {
            init(form, mipLevels, size, numLayers);
        }
        void init(TexFormat form, GLint mipLevels, const glm::ivec3 & size) {
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

        ImmutableTexture3d(TexComponent comp, TexFormat form, GLuint levels, const glm::ivec3 & size)
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