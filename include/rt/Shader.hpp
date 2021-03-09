#pragma once
#include "Core.hpp"
#include <string_view>
#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <limits>

namespace rt {
	enum class ShaderStage {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Compute = GL_COMPUTE_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
	};

	class Shader {
	public:
		Shader(ShaderStage t)
			: id(0)
			, kind(t)
		{
			id = glCreateShader((GLenum)kind); 
			checkError();
		}
		Shader(ShaderStage t, std::string_view source): id(0), kind(t)
		{
			id = glCreateShader((GLenum)kind); 
			checkError();

			addSource(source);
		}

		~Shader() {
			if(id != 0) {
				glDeleteShader(id); 
				checkError();
				id = 0;
			}
		}

		void addSource(std::string_view source) {
			const char* sourcePtr = source.data();
			assert(source.length() < std::numeric_limits<int>::max());
			GLint sourceLength = static_cast<int>(source.length());

			glShaderSource(id, 1, &sourcePtr, &sourceLength); 
			checkError();
		}

		bool isCompiled() const {
			GLint result = GL_FALSE;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result); 
			checkError();
			return result == GL_TRUE;
		}

		bool compile(std::string_view src) {
			addSource(src);
			return compile();
		}

		bool compile() {
			glCompileShader(id);
			bool compiled = isCompiled();

			if (!compiled)
			{
				int log_len;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);

				std::string log_space(static_cast<size_t>(log_len) + static_cast<size_t>(1), ' ');
				glGetShaderInfoLog(id, log_len, nullptr, (GLchar*)log_space.c_str());

				std::cerr << "Compile failure in " <<
					(kind == ShaderStage::Vertex ? "vertex" : "fragment") <<
					" shader: " << log_space << std::endl;

				checkError();
			}

			return compiled;
		}

		void reset() {
			if(isValid()){
				glDeleteShader(id); 
				checkError();
				id = glCreateShader((GLenum)kind); 
				checkError();
			}
			else {
				id = glCreateShader((GLenum)kind); 
				checkError();
			}
		}
		void reset(ShaderStage stage) {
			kind = stage;
			reset();
		}

		ShaderStage getType() const {
			return kind;
		}

		bool isValid() const {
			return id != 0;
		}

		GLuint getId() const {
			return id;
		}

		bool verifyId() const {
			int result = glIsShader(id); 
			checkError();
			return result == GL_TRUE;
		}
	protected:
		GLuint id;
		ShaderStage kind;
	};
}


