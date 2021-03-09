#pragma once
#include "Core.hpp"
#include "TextureUtilities.hpp"
#include <cassert>

namespace rt {
	class RenderBuffer {
	public:
		RenderBuffer()
			: id(0)
			, width(0)
			, height(0)
			, format(TexFormat::R_N8)
		{
			glCreateRenderbuffers(1, &id);
			checkError();
		}
		~RenderBuffer() {
			if (isValid()) {
				glDeleteRenderbuffers(1, &id);
				checkError();
				id = 0;
			}
		}

		RenderBuffer(RenderBuffer&& other) noexcept 
			: id(other.getId())
			, width(other.width)
			, height(other.height)
			, format(other.format)
		{
			other.id = 0;
		}
		RenderBuffer& operator=(RenderBuffer&& other) noexcept {
			if (isValid()) {
				glDeleteRenderbuffers(1, &id);
				checkError();
			}
			id = other.getId();
			width = other.getWidth();
			other.width = 0;
			height = other.getHeight();
			other.height = 0;

			format = other.format;

			other.id = 0;
			return *this;
		}

		bool isValid() const noexcept {
			return id != 0;
		}
		bool isInitialized() const noexcept {
			return width != 0;
		}

		void init(TexFormat form, const glm::ivec2 & size) {
			assert(isValid());
			assert(width == 0);
			assert(size.x > 0);
			assert(size.y > 0);

			format = form;
			GLenum formEnum = convertGL(format);

			width = size.x;
			height = size.y;
			glNamedRenderbufferStorage(id, formEnum, width, height);
			checkError();
		}

		void initMultiSample(GLint samples, TexFormat form, const glm::ivec2 & size) {
			assert(isValid());
			assert(width == 0);
			assert(size.x > 0);
			assert(size.y > 0);

			format = form;
			GLenum formEnum = convertGL(format);

			width = size.x;
			height = size.y;
			glNamedRenderbufferStorageMultisample(id, samples, formEnum, width, height); 
			checkError();
		}

		void reset() {
			if (isValid()) {
				glDeleteRenderbuffers(1, &id); 
				checkError();
				glCreateRenderbuffers(1, &id); 
				checkError();
				width = 0;
				height = 0;
			}
		}

		GLint getWidth() const noexcept {
			return width;
		}
		GLint getHeight() const noexcept {
			return height;
		}
		glm::ivec2 getSize() const noexcept {
			return glm::ivec2(width, height);
		}

		GLuint getId() const noexcept {
			return id;
		}
	private:
		GLuint id;
		GLint width, height;
		TexFormat format;
	};
};