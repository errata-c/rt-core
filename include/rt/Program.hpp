#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <stdexcept>

#include "Core.hpp"
#include "Shader.hpp"
#include "BindlessTexture.hpp"

namespace rt {
	enum class UniformType : GLenum {
		Float = GL_FLOAT,
		Vec2 = GL_FLOAT_VEC2,
		Vec3 = GL_FLOAT_VEC3,
		Vec4 = GL_FLOAT_VEC4,

		Double = GL_DOUBLE,
		DVec2 = GL_DOUBLE_VEC2,
		DVec3 = GL_DOUBLE_VEC3,
		DVec4 = GL_DOUBLE_VEC4,

		Int = GL_INT,
		IVec2 = GL_INT_VEC2,
		IVec3 = GL_INT_VEC3,
		IVec4 = GL_INT_VEC4,

		UInt = GL_UNSIGNED_INT,
		UVec2 = GL_UNSIGNED_INT_VEC2,
		UVec3 = GL_UNSIGNED_INT_VEC3,
		UVec4 = GL_UNSIGNED_INT_VEC4,

		Bool = GL_BOOL,
		BVec2 = GL_BOOL_VEC2,
		BVec3 = GL_BOOL_VEC3,
		BVec4 = GL_BOOL_VEC4,

		Mat2 = GL_FLOAT_MAT2,
		Mat3 = GL_FLOAT_MAT3,
		Mat4 = GL_FLOAT_MAT4,
		Mat2x3 = GL_FLOAT_MAT2x3,
		Mat2x4 = GL_FLOAT_MAT2x4,
		Mat3x2 = GL_FLOAT_MAT3x2,
		Mat3x4 = GL_FLOAT_MAT3x4,
		Mat4x2 = GL_FLOAT_MAT4x2,
		Mat4x3 = GL_FLOAT_MAT4x3,

		DMat2 = GL_DOUBLE_MAT2,
		DMat3 = GL_DOUBLE_MAT3,
		DMat4 = GL_DOUBLE_MAT4,
		DMat2x3 = GL_DOUBLE_MAT2x3,
		DMat2x4 = GL_DOUBLE_MAT2x4,
		DMat3x2 = GL_DOUBLE_MAT3x2,
		DMat3x4 = GL_DOUBLE_MAT3x4,
		DMat4x2 = GL_DOUBLE_MAT4x2,
		DMat4x3 = GL_DOUBLE_MAT4x3,

		Sampler1D = GL_SAMPLER_1D,
		Sampler2D = GL_SAMPLER_2D,
		Sampler3D = GL_SAMPLER_3D,
		SamplerCube = GL_SAMPLER_CUBE,
		Sampler1DShadow = GL_SAMPLER_1D_SHADOW,
		Sampler2DShadow = GL_SAMPLER_2D_SHADOW,
		Sampler1DArray = GL_SAMPLER_1D_ARRAY,
		Sampler2DArray = GL_SAMPLER_2D_ARRAY,
		Sampler2DMS = GL_SAMPLER_2D_MULTISAMPLE,
		Sampler2DMSArray = GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
		SamplerCubeShadow = GL_SAMPLER_CUBE_SHADOW,
		SamplerBuffer = GL_SAMPLER_BUFFER,
		Sampler2DRect = GL_SAMPLER_2D_RECT,
		Sampler2DRectShadow = GL_SAMPLER_2D_RECT_SHADOW,

		ISampler1D = GL_INT_SAMPLER_1D,
		ISampler2D = GL_INT_SAMPLER_2D,
		ISampler3D = GL_INT_SAMPLER_3D,
		ISamplerCube = GL_INT_SAMPLER_CUBE,
		ISampler1DArray = GL_INT_SAMPLER_1D_ARRAY,
		ISampler2DArray = GL_INT_SAMPLER_2D_ARRAY,
		ISampler2DMS = GL_INT_SAMPLER_2D_MULTISAMPLE,
		ISampler2DMSArray = GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		ISamplerBuffer = GL_INT_SAMPLER_BUFFER,
		ISampler2DRect = GL_INT_SAMPLER_2D_RECT,

		USampler1D = GL_UNSIGNED_INT_SAMPLER_1D,
		USampler2D = GL_UNSIGNED_INT_SAMPLER_2D,
		USampler3D = GL_UNSIGNED_INT_SAMPLER_3D,
		USamplerCube = GL_UNSIGNED_INT_SAMPLER_CUBE,
		USampler1DArray = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
		USampler2DArray = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
		USampler2DMS = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
		USampler2DMSArray = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		USamplerBuffer = GL_UNSIGNED_INT_SAMPLER_BUFFER,
		USampler2DRect = GL_UNSIGNED_INT_SAMPLER_2D_RECT,

		Image1D = GL_IMAGE_1D,
		Image2D = GL_IMAGE_2D,
		Image3D = GL_IMAGE_3D,
		Image2DRect = GL_IMAGE_2D_RECT,
		ImageCube = GL_IMAGE_CUBE,
		ImageBuffer = GL_IMAGE_BUFFER,
		Image1DArray = GL_IMAGE_1D_ARRAY,
		Image2DArray = GL_IMAGE_2D_ARRAY,
		Image2DMS = GL_IMAGE_2D_MULTISAMPLE,
		Image2DMSArray = GL_IMAGE_2D_MULTISAMPLE_ARRAY,

		IImage1D = GL_INT_IMAGE_1D,
		IImage2D = GL_INT_IMAGE_2D,
		IImage3D = GL_INT_IMAGE_3D,
		IImage2DRect = GL_INT_IMAGE_2D_RECT,
		IImageCube = GL_INT_IMAGE_CUBE,
		IImageBuffer = GL_INT_IMAGE_BUFFER,
		IImage1DArray = GL_INT_IMAGE_1D_ARRAY,
		IImage2DArray = GL_INT_IMAGE_2D_ARRAY,
		IImage2DMS = GL_INT_IMAGE_2D_MULTISAMPLE,
		IImage2DMSArray = GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
		
		UImage1D = GL_UNSIGNED_INT_IMAGE_1D,
		UImage2D = GL_UNSIGNED_INT_IMAGE_2D,
		UImage3D = GL_UNSIGNED_INT_IMAGE_3D,
		UImage2DRect = GL_UNSIGNED_INT_IMAGE_2D_RECT,
		UImageCube = GL_UNSIGNED_INT_IMAGE_CUBE,
		UImageBuffer = GL_UNSIGNED_INT_IMAGE_BUFFER,
		UImage1DArray = GL_UNSIGNED_INT_IMAGE_1D_ARRAY,
		UImage2DArray = GL_UNSIGNED_INT_IMAGE_2D_ARRAY,
		UImage2DMS = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
		UImage2DMSArray = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,

		AtomicCounter = GL_UNSIGNED_INT_ATOMIC_COUNTER,
	};


class Program {
public:
	static GLuint CurrentId() {
		GLint value = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &value);
		checkError();
		return value;
	}

	Program(Shader & vertShader, Shader & fragShader) 
		: id(0)
		, bound(false)
	{
		id = glCreateProgram();
		checkError();
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
			id = 0;
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

	GLint getInputLocation(std::string_view name) const {
		// std::string_view and c-strings are not the same, so we have to check. This is opengl's fault. >:(
		if (*(name.data() + name.length()) != '\0') {
			throw std::invalid_argument("Argument 'name' for rt::Program::getSSBOIndex is not null terminated!");
		}
		GLint tmp = glGetProgramResourceLocation(id, GL_PROGRAM_INPUT, name.data());
		checkError();
		return tmp;
	}
	GLint getOutputLocation(std::string_view name) const {
		if (*(name.data() + name.length()) != '\0') {
			throw std::invalid_argument("Argument 'name' for rt::Program::getSSBOIndex is not null terminated!");
		}
		GLint tmp = glGetProgramResourceLocation(id, GL_PROGRAM_OUTPUT, name.data());
		checkError();
		return tmp;
	}
	GLint getUniformLocation(std::string_view name) const {
		if (*(name.data() + name.length()) != '\0') {
			throw std::invalid_argument("Argument 'name' for rt::Program::getSSBOIndex is not null terminated!");
		}
		GLint tmp = glGetProgramResourceLocation(id, GL_UNIFORM, name.data());
		checkError();
		return tmp;
	}

	GLint getUBOIndex(std::string_view name) const {
		if (*(name.data() + name.length()) != '\0') {
			throw std::invalid_argument("Argument 'name' for rt::Program::getSSBOIndex is not null terminated!");
		}
		GLint tmp = glGetProgramResourceIndex(id, GL_UNIFORM_BLOCK, name.data());
		checkError();
		return tmp;
	}
	GLint getSSBOIndex(std::string_view name) const {
		if (*(name.data() + name.length()) != '\0') {
			throw std::invalid_argument("Argument 'name' for rt::Program::getSSBOIndex is not null terminated!");
		}
		GLint tmp = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name.data());
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

		glGetProgramInfoLog(id, len, nullptr, result.data());
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