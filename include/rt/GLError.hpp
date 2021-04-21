#pragma once
#include "Core.hpp"
#include <fmt/core.h>

namespace rt {
	namespace intern {
		template<std::size_t N>
		std::string_view makeView(const char(&text)[N]) {
			return std::string_view{ text, N - 1 };
		}
	}
	static std::string_view getErrorString(unsigned code) {
		switch (code) {
		case GL_NO_ERROR:
			return intern::makeView("GL_NO_ERROR");
			break;
		case GL_INVALID_ENUM:
			return intern::makeView("GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			return intern::makeView("GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			return intern::makeView("GL_INVALID_OPERATION");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return intern::makeView("GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		case GL_OUT_OF_MEMORY:
			return intern::makeView("GL_OUT_OF_MEMORY");
			break;
		default:
			return intern::makeView("Unrecognized error code");
			break;
		}
	}

	static void printLastError() {
		uint32_t code = glGetError();
		std::cerr << getErrorString(code) << '\n';
	}

	static std::string_view getLastErrorString() {
		return getErrorString(glGetError());
	}

	static void checkError() {
#ifdef RENDER_TOOLS_ERROR_CHECKS
		uint32_t code = glGetError();
		if (code != GL_NO_ERROR) {
			fmt::print(stderr, "OpenGL error encountered!\n{}\n", getErrorString(code));
			throw std::exception(getErrorString(code).data());
		}
#endif
	}
}
