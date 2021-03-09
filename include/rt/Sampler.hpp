#pragma once
#include "Core.hpp"

namespace rt {
	class Sampler {
	public:
		Sampler()
			: id(0)
		{
			glCreateSamplers(1, &id); 
			checkError();
		}

		~Sampler()
		{
			if (isValid()) {
				glDeleteSamplers(1, &id);
				checkError();
				id = 0;
			}
		}

		void bindUnit(GLuint texUnit) {
			glBindSampler(id, texUnit); 
			checkError();
		}

		void unbindUnit(GLuint texUnit) {
			glBindSampler(0, texUnit); 
			checkError();
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

		void clampEdge() {
			clampEdgeS();
			clampEdgeT();
			clampEdgeR();
		}
		void clampEdgeS() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			checkError();
		}
		void clampEdgeT() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			checkError();
		}
		void clampEdgeR() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
			checkError();
		}

		void clampBorder() {
			clampBorderS();
			clampBorderT();
			clampBorderR();
		}
		void clampBorderS() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
			checkError();
		}
		void clampBorderT() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
			checkError();
		}
		void clampBorderR() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER); 
			checkError();
		}

		void makeRepeat() {
			makeRepeatS();
			makeRepeatT();
			makeRepeatR();
		}
		void makeRepeatS() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT); 
			checkError();
		}
		void makeRepeatT() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT); 
			checkError();
		}
		void makeRepeatR() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_REPEAT); 
			checkError();
		}

		void makeMirroredRepeat() {
			makeMirroredRepeatS();
			makeMirroredRepeatT();
			makeMirroredRepeatR();
		}
		void makeMirroredRepeatS() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); 
			checkError();
		}
		void makeMirroredRepeatT() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
			checkError();
		}
		void makeMirroredRepeatR() {
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT); 
			checkError();
		}


		void minFilterNearest() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
			checkError();
		}
		void minFilterLinear() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
			checkError();
		}
		void minFilterNearestMipmapNearest() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); 
			checkError();
		}
		void minFilterNearestMipmapLinear() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); 
			checkError();
		}
		void minFilterLinearMipmapLinear() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
			checkError();
		}
		void minFilterLinearMipmapNearest() {
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
			checkError();
		}

		void magFilterNearest() {
			glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
			checkError();
		}
		void magFilterLinear() {
			glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
			checkError();
		}

		bool isValid() const {
			return id != 0;
		}

		GLuint getId() const {
			return id;
		}
	private:
		GLuint id;
	};
}