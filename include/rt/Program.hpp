#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>

#include "Core.hpp"
#include "Shader.hpp"
#include "BindlessTexture.hpp"

namespace rt {

class Program {
public:
	Program(Shader & vertShader, Shader & fragShader) 
		: id(0)
		, bound(false)
	{
		id = glCreateProgram();
		assert(isValid());

		compile(vertShader, fragShader);
	}
	Program()
		: id(0)
		, bound(false)
	{
		id = glCreateProgram();
		assert(isValid());
	}
	~Program() {
		if (id != 0) {
			glDeleteProgram(id);
			checkError();
		}
	}

	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	Program(Program&& other) noexcept
		: id(other.id)
		, bound(other.bound)
	{
		other.id = 0;
		other.bound = false;
	}

	Program& operator=(Program&& other) noexcept {
		if (isValid()) {
			glDeleteProgram(id);
			checkError();

			id = glCreateProgram();
			checkError();
		}

		id = other.id;
		bound = other.bound;

		other.id = 0;
		other.bound = false;
		return *this;
	}

	GLint getInputLocation(const char * name) const {
		GLint tmp = glGetProgramResourceLocation(id, GL_PROGRAM_INPUT, name);
		checkError();
		return tmp;
	}
	GLint getOutputLocation(const char* name) const {
		GLint tmp = glGetProgramResourceLocation(id, GL_PROGRAM_OUTPUT, name);
		checkError();
		return tmp;
	}
	GLint getUniformLocation(const char* name) const {
		GLint tmp = glGetProgramResourceLocation(id, GL_UNIFORM, name);
		checkError();
		return tmp;
	}

	GLint getUBOIndex(const char* name) const {
		GLint tmp = glGetProgramResourceIndex(id, GL_UNIFORM_BLOCK, name);
		checkError();
		return tmp;
	}
	GLint getSSBOIndex(const char* name) const {
		GLint tmp = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name);
		checkError();
		return tmp;
	}

	void bindUBOIndex(GLint uboIndex, GLuint bindingIndex) {
		glUniformBlockBinding(id, uboIndex, bindingIndex);
	}
	void bindSSBOIndex(GLint ssboIndex, GLuint bindingIndex) {
		glShaderStorageBlockBinding(id, ssboIndex, bindingIndex);
	}

	void uniform(GLint location, bool val) {
		GLuint converted = val;
		uniform(location, &converted, 1);
	}

	void uniform(GLint location, const GLfloat val) {
		glProgramUniform1f(id, location, val);
		checkError();
	}
	void uniform(GLint location, const GLfloat * vals, GLsizei count) {
		glProgramUniform1fv(id, location, count, vals);
		checkError();
	}

	void uniform(GLint location, const GLuint vals) {
		glProgramUniform1ui(id, location, vals);
		checkError();
	}
	void uniform(GLint location, const GLuint * vals, GLsizei count) {
		glProgramUniform1uiv(id, location, count, vals);
		checkError();
	}

	void uniform(GLint location, const GLint vals) {
		glProgramUniform1i(id, location, vals);
		checkError();
	}
	void uniform(GLint location, const GLint * vals, GLsizei count) {
		glProgramUniform1iv(id, location, count, vals);
		checkError();
	}

	void uniformBindless(GLint location, const BindlessTexture & tex) {
		glProgramUniformHandleui64ARB(id, location, tex.getHandle());
		checkError();
	}
	void uniformBindless(GLint location, const GLuint64 vals) {
		glProgramUniformHandleui64ARB(id, location, vals);
		checkError();
	}
	void uniformBindless(GLint location, const GLuint64* vals, GLsizei count) {
		glProgramUniformHandleui64vARB(id, location, count, vals);
		checkError();
	}
	
	void uniformSampler(GLint location, const GLint vals) {
		glProgramUniform1i(id, location, vals);
		checkError();
	}
	void uniformSampler(GLint location, const GLint * vals, GLsizei count) {
		glProgramUniform1iv(id, location, count, vals);
		checkError();
	}

	void uniform(GLint location, const glm::vec2& vals) {
		glProgramUniform2f(id, location, vals.x, vals.y);
		checkError();
	}
	void uniform(GLint location, const glm::vec3& vals) {
		glProgramUniform3f(id, location, vals.x, vals.y, vals.z);
		checkError();
	}
	void uniform(GLint location, const glm::vec4& vals) {
		glProgramUniform4f(id, location, vals.x, vals.y, vals.z, vals.w);
		checkError();
	}

	void uniform(GLint location, const glm::vec2* vals, GLsizei count) {
		glProgramUniform2fv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::vec3* vals, GLsizei count) {
		glProgramUniform3fv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::vec4* vals, GLsizei count) {
		glProgramUniform4fv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}


	void uniform(GLint location, const glm::uvec2& vals) {
		glProgramUniform2ui(id, location, vals.x, vals.y);
		checkError();
	}
	void uniform(GLint location, const glm::uvec3& vals) {
		glProgramUniform3ui(id, location, vals.x, vals.y, vals.z);
		checkError();
	}
	void uniform(GLint location, const glm::uvec4& vals) {
		glProgramUniform4ui(id, location, vals.x, vals.y, vals.z, vals.w);
		checkError();
	}

	void uniform(GLint location, const glm::uvec2* vals, GLsizei count) {
		glProgramUniform2uiv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::uvec3* vals, GLsizei count) {
		glProgramUniform3uiv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::uvec4* vals, GLsizei count) {
		glProgramUniform4uiv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}


	void uniform(GLint location, const glm::ivec2& vals) {
		glProgramUniform2i(id, location, vals.x, vals.y);
		checkError();
	}
	void uniform(GLint location, const glm::ivec3& vals) {
		glProgramUniform3i(id, location, vals.x, vals.y, vals.z);
		checkError();
	}
	void uniform(GLint location, const glm::ivec4& vals) {
		glProgramUniform4i(id, location, vals.x, vals.y, vals.z, vals.w);
		checkError();
	}

	void uniform(GLint location, const glm::ivec2* vals, GLsizei count) {
		glProgramUniform2iv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::ivec3* vals, GLsizei count) {
		glProgramUniform3iv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::ivec4* vals, GLsizei count) {
		glProgramUniform4iv(id, location, count, glm::value_ptr(vals[0]));
		checkError();
	}

	void uniform(GLint location, const glm::mat2x2& vals, bool transpose = false) {
		glProgramUniformMatrix2fv(id, location, 1, transpose, glm::value_ptr(vals));
		checkError();
	}
	void uniform(GLint location, const glm::mat2x3& vals, bool transpose = false) {
		glProgramUniformMatrix2x3fv(id, location, 1, transpose, glm::value_ptr(vals));
		checkError();
	}
	void uniform(GLint location, const glm::mat2x4& vals, bool transpose = false) {
		glProgramUniformMatrix2x4fv(id, location, 1, transpose, glm::value_ptr(vals));
		checkError();
	}

	void uniform(GLint location, const glm::mat2x2* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix2fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat2x3* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix2x3fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat2x4* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix2x4fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}


	void uniform(GLint location, const glm::mat3x2& vals, bool transpose = false) {
		glProgramUniformMatrix3x2fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat3x3& vals, bool transpose = false) {
		glProgramUniformMatrix3fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat3x4& vals, bool transpose = false) {
		glProgramUniformMatrix3x4fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}

	void uniform(GLint location, const glm::mat3x2* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix3x2fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat3x3* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix3fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat3x4* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix3x4fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}


	void uniform(GLint location, const glm::mat4x2& vals, bool transpose = false) {
		glProgramUniformMatrix4x2fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat4x3& vals, bool transpose = false) {
		glProgramUniformMatrix4x3fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat4x4& vals, bool transpose = false) {
		glProgramUniformMatrix4fv(id, location, 1, transpose, glm::value_ptr(vals[0]));
		checkError();
	}

	void uniform(GLint location, const glm::mat4x2* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix4x2fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat4x3* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix4x3fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}
	void uniform(GLint location, const glm::mat4x4* vals, GLsizei count, bool transpose = false) {
		glProgramUniformMatrix4fv(id, location, count, transpose, glm::value_ptr(vals[0]));
		checkError();
	}

	void attachShader(const Shader & shader) {
		assert(shader.isCompiled());
		glAttachShader(id, shader.getId());
		checkError();
	}
	void detachShader(const Shader& shader) {
		glDetachShader(id, shader.getId());
		checkError();
	}

	bool isLinked() const {
		GLint result = GL_FALSE;
		glGetProgramiv(id, GL_LINK_STATUS, &result);
		checkError();
		return result == GL_TRUE;
	}

	int numAttachedShaders() const {
		GLint result = 0;
		glGetProgramiv(id, GL_ATTACHED_SHADERS, &result);
		checkError();
		return result;
	}
	int numActiveUniforms() const {
		GLint result = 0;
		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &result);
		checkError();
		return result;
	}
	int numActiveAttributes() const {
		GLint result = 0;
		glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &result);
		checkError();
		return result;
	}

	std::string getInfoLog() const {
		std::string result;
		GLint len = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
		checkError();
		result.resize(len);

		glGetProgramInfoLog(id, result.size(), nullptr, result.data());
		checkError();
		return result;
	}

	void bind() {
		assert(isValid() && "There is no program! Compilation either failed or did not occur.");
		glUseProgram(id);
		checkError();
		bound = true;
	}
	void unbind() {
		glUseProgram(0);
		checkError();
		bound = false;
	}

	bool isBound() const {
		return bound;
	}

	bool isValid() const {
		return id != 0;
	}

	void reset() {
		if(isValid()) {
			glDeleteProgram(id);
			checkError();
			id = glCreateProgram();
			checkError();
			assert(id != 0);
		}
		else {
			id = glCreateProgram();
			checkError();
		}
	}

	bool compile(Shader & vertShader, Shader & fragShader) {
		if (!vertShader.isValid() || !fragShader.isValid()) {
			return false;
		}

		assert(vertShader.getType() == ShaderStage::Vertex);
		assert(fragShader.getType() == ShaderStage::Fragment);

		assert(vertShader.isCompiled());
		assert(fragShader.isCompiled());

		assert(isValid());

		attachShader(vertShader);
		attachShader(fragShader);

		return compile();
	}

	bool compile(std::string_view vertexSource, std::string_view fragmentSource) {
		Shader vertShader(ShaderStage::Vertex, vertexSource);
		Shader fragShader(ShaderStage::Fragment, fragmentSource);

		vertShader.compile();
		fragShader.compile();

		return compile(vertShader, fragShader);
	}

	bool compile() {
		assert(!isLinked() && "Program was already linked!");

		glLinkProgram(id);

		if (!isLinked()) {

			std::string log = getInfoLog();
			if (log.empty()) {
				std::cerr << "Linking failure with no error log." << std::endl;
			}
			else {
				std::cerr << "Linking failure: \n" << log << std::endl;
			}

			return false;
		}
		else {
			return true;
		}
	}

	GLuint getId() const {
		return id;
	}

	bool verifyId() const {
		int result = glIsProgram(id);
		checkError();
		return result == GL_TRUE;
	}
protected:
	GLuint id;
	bool bound;
};

}