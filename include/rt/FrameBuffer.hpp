#pragma once
#include "Core.hpp"
#include "Texture.hpp"
#include "RenderBuffer.hpp"

namespace rt {
	enum class FrameBufferStatus {
		Complete = GL_FRAMEBUFFER_COMPLETE,
		Undefined = GL_FRAMEBUFFER_UNDEFINED,
		IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
		MissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
		Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
		IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
		IncompleteLayer = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
		IncompleteDraw = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
		IncompleteRead = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
	};
	using FBStatus = FrameBufferStatus;

	enum class FrameBufferAttachment {
		Color = GL_COLOR_ATTACHMENT0,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
	};
	using FBAttach = FrameBufferAttachment;

	enum class FrameBufferMask {
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT,
		All = Color | Depth | Stencil,
	};
	using FBMask = FrameBufferMask;

	class FrameBuffer {
	public:
		using Status = FrameBufferStatus;
		using Attachment = FrameBufferAttachment;
		
		static GLuint CurrentReadId() {
			GLint value = 0;
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &value);
			return value;
		}
		static GLuint CurrentDrawId() {
			GLint value = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &value);
			return value;
		}

		static constexpr Status
			Complete = Status::Complete,
			Undefined = Status::Undefined,
			IncompleteAttachment = Status::IncompleteAttachment,
			MissingAttachment = Status::MissingAttachment,
			Unsupported = Status::Unsupported,
			IncompleteMultisample = Status::IncompleteMultisample,
			IncompleteLayer = Status::IncompleteLayer,
			IncompleteDraw = Status::IncompleteDraw,
			IncompleteRead = Status::IncompleteRead;

		FrameBuffer() {
			glCreateFramebuffers(1, &id); 
			checkError();
		}
		~FrameBuffer() {
			if (isValid()) {
				glDeleteFramebuffers(1, &id); 
				checkError();
				id = 0;
			}
		}

		bool isValid() const {
			return id != 0;
		}
		bool isComplete() const {
			bool res = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			checkError();
			return res;
		}
		FrameBufferStatus status() const {
			GLenum tmp = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
			checkError();
			return static_cast<FrameBufferStatus>(tmp);
		}

		void detach(FBAttach attach) {
			glNamedFramebufferRenderbuffer(id, (int)attach, GL_RENDERBUFFER, 0); 
			checkError();
		}
		void detachColor(GLint attachI) {
			glNamedFramebufferRenderbuffer(id, GL_COLOR_ATTACHMENT0 + attachI, GL_RENDERBUFFER, 0); 
			checkError();
		}

		void attachColor(Texture3dBase& tex, GLint attachI, GLint layer, GLint level = 0) {
			glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + attachI, tex.getId(), level, layer); 
			checkError();
		}
		void attach(Texture3dBase& tex, FBAttach attach, GLint layer, GLint level = 0) {
			glNamedFramebufferTextureLayer(id, (int)attach, tex.getId(), level, layer); 
			checkError();
		}

		void attachColor(Texture2dBase & tex, GLint attachI, GLint level = 0) {
			glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + attachI, tex.getId(), level); 
			checkError();
		}
		void attach(Texture2dBase & tex, FBAttach attach, GLint level = 0) {
			glNamedFramebufferTexture(id, (int)attach, tex.getId(), level); 
			checkError();
		}

		void attachColor(RenderBuffer& tex, GLint attachI) {
			glNamedFramebufferRenderbuffer(id, GL_COLOR_ATTACHMENT0 + attachI, GL_RENDERBUFFER, tex.getId()); 
			checkError();
		}
		void attach(RenderBuffer& tex, FBAttach attach) {
			glNamedFramebufferRenderbuffer(id, (int)attach, GL_RENDERBUFFER, tex.getId()); 
			checkError();
		}

		void blitTo(FrameBuffer& other, const glm::ivec2& read, const glm::ivec2& write, const glm::ivec2& region, FBMask mask = FBMask::All) {
			glBlitNamedFramebuffer(id, other.id, read.x, read.y, read.x + region.x, read.y + region.y, write.x, write.y, write.x + region.x, write.y + region.y, (GLbitfield)mask, GL_NEAREST);
			checkError();
		}
		void blitTo(FrameBuffer& other, const glm::ivec2 & read, const glm::ivec2& readRegion, const glm::ivec2& write, const glm::ivec2 & writeRegion, FBMask mask = FBMask::All) {
			glBlitNamedFramebuffer(id, other.id, read.x, read.y, read.x + readRegion.x, read.y + readRegion.y, write.x, write.y, write.x + writeRegion.x, write.y + writeRegion.y, (GLbitfield)mask, GL_NEAREST);
			checkError();
		}

		void blitDefault(const glm::ivec2& read, const glm::ivec2& write, const glm::ivec2& region, FBMask mask = FBMask::All) {
			glBlitNamedFramebuffer(id, 0, read.x, read.y, read.x + region.x, read.y + region.y, write.x, write.y, write.x + region.x, write.y + region.y, (GLbitfield)mask, GL_NEAREST);
			checkError();
		}
		void blitDefault(const glm::ivec2& read, const glm::ivec2& readRegion, const glm::ivec2& write, const glm::ivec2& writeRegion, FBMask mask = FBMask::All) {
			glBlitNamedFramebuffer(id, 0, read.x, read.y, read.x + readRegion.x, read.y + readRegion.y, write.x, write.y, write.x + writeRegion.x, write.y + writeRegion.y, (GLbitfield)mask, GL_NEAREST);
			checkError();
		}

		void bind() {
			assert(isValid());
			glBindFramebuffer(GL_FRAMEBUFFER, id); 
			checkError();
		}
		void unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); 
			checkError();
		}

		void bindRead() {
			assert(isValid());
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id); 
			checkError();
		}
		void unbindRead() {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); 
			checkError();
		}

		void bindDraw() {
			assert(isValid());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id); 
			checkError();
		}
		void unbindDraw() {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
			checkError();
		}

		GLuint getId() const {
			return id;
		}

		void reset() {
			if (isValid()) {
				glDeleteFramebuffers(1, &id); checkError();
			}
			glCreateFramebuffers(1, &id); checkError();
		}
	private:
		GLuint id;
	};
}

inline rt::FBMask operator|(rt::FBMask lh, rt::FBMask rh) {
	return (rt::FBMask)((int)lh | (int)rh);
}
inline rt::FBMask operator&(rt::FBMask lh, rt::FBMask rh) {
	return (rt::FBMask)((int)lh & (int)rh);
}
inline rt::FBMask& operator|=(rt::FBMask& lh, rt::FBMask rh) {
	lh = (rt::FBMask)((int)lh | (int)rh);
	return lh;
}
inline rt::FBMask& operator&=(rt::FBMask& lh, rt::FBMask rh) {
	lh = (rt::FBMask)((int)lh & (int)rh);
	return lh;
}