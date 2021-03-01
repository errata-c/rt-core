#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <array>
#include <cinttypes>
#include <cassert>
#include <cstdio>
#include <rt/loader.hpp>

#include <rt/GLError.hpp>

namespace rt {
	enum class Type {
		Half = GL_HALF_FLOAT,
		Float = GL_FLOAT,

		U8 = GL_UNSIGNED_BYTE,
		U16 = GL_UNSIGNED_SHORT,
		U32 = GL_UNSIGNED_INT,

		I8 = GL_BYTE,
		I16 = GL_SHORT,
		I32 = GL_INT
	};

	enum class Index {
		U8 = GL_UNSIGNED_BYTE,
		U16 = GL_UNSIGNED_SHORT,
		U32 = GL_UNSIGNED_INT,
	};

	static GLenum convertGL(Type type) noexcept {
		return static_cast<GLenum>(type);
	}
	static GLenum convertGL(Index index) noexcept {
		return static_cast<GLenum>(index);
	}
}