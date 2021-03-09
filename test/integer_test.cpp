#include <rt/Program.hpp>
#include <rt/Buffer.hpp>
#include <rt/VertexArray.hpp>
#include <rt/GLError.hpp>

#include <SFML/Window.hpp>
#include <cinttypes>
#include <iostream>
#include <cassert>
#include <fmt/printf.h>

#define USE_RENDER_TOOLS

using BufferFlags = rt::BufferFlags;

int main() {
	const char* vertexSource = R"foo(
#version 450
layout(location = 0) in uint index;
out vec4 fragColor;

uniform vec4 color;
uniform vec2 position;
uniform vec2 scale;

vec3 box[4] = vec3[4](
	vec3(-1, +1, +1),
	vec3(+1, +1, +1),
	vec3(-1, -1, +1),
	vec3(+1, -1, +1)
);

void main() {
	uint subIndex = index % 4;
	
	vec3 bPos = box[subIndex];

	mat3 model = mat3(
		vec3(scale.x, 0, 0), // column 0 
		vec3(0, scale.y, 0), // column 1
		vec3(position,   0)  // column 2
	);

	gl_Position = vec4(model * bPos, 1);
	fragColor = color;
}
)foo";

	const char* fragSource = R"foo(
#version 450
in vec4 fragColor;

out vec4 finalColor;

void main() {
	finalColor = fragColor;
}
)foo";

	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.depthBits = 0;
	settings.stencilBits = 0;

	sf::Window window{ sf::VideoMode{800, 600}, "integer_test", sf::Style::Default, settings };
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	settings = window.getSettings();
	assert(settings.majorVersion == 4);
	assert(settings.minorVersion == 5);
	{
		glClearColor(0.4f, 0.4f, 0.6f, 1.f);
		glViewport(0, 0, 800, 600);

		GLuint
			vaoId = 0,
			vbId = 0,
			programId = 0,
			vertShader = 0,
			fragShader = 0;

		// SECTION 0:
#ifndef USE_RENDER_TOOLS
		glCreateVertexArrays(1, &vaoId);
		glCreateBuffers(1, &vbId);

		programId = glCreateProgram();

		vertShader = glCreateShader(GL_VERTEX_SHADER);
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertShader, 1, &vertexSource, NULL);
		glShaderSource(fragShader, 1, &fragSource, NULL);

		glCompileShader(vertShader);
		glCompileShader(fragShader);

		glAttachShader(programId, vertShader);
		glAttachShader(programId, fragShader);

		glLinkProgram(programId);

		GLint
			colorLoc = glGetProgramResourceLocation(programId, GL_UNIFORM, "color"),
			positionLoc = glGetProgramResourceLocation(programId, GL_UNIFORM, "position"),
			scaleLoc = glGetProgramResourceLocation(programId, GL_UNIFORM, "scale");

		bool isProgram = glIsProgram(programId);
#else
		rt::VertexArray vao;
		rt::ImmutableBuffer vbo;

		rt::Program program;
		program.compile(vertexSource, fragSource);

		GLint
			colorLoc = program.getUniformLocation("color"),
			positionLoc = program.getUniformLocation("position"),
			scaleLoc = program.getUniformLocation("scale");

		bool isProgram = glIsProgram(program.getId());
#endif
		fmt::print("Section 0 error: {}\n", rt::getLastErrorString());
		fmt::print("isProgram == {}\n", isProgram);
		fmt::print("colorLoc == {}, positionLoc == {}, scaleLoc == {}\n", colorLoc, positionLoc, scaleLoc);

		glm::vec4 color{ 0, 0, 0, 1 };
		glm::vec2 position{ 0, 0 };
		glm::vec2 scale{ 0.5f, 0.5f };

		// SECTION 1:
#ifndef USE_RENDER_TOOLS
		glProgramUniform4fv(programId, colorLoc, 1, glm::value_ptr(color));
		glProgramUniform2fv(programId, positionLoc, 1, glm::value_ptr(position));
		glProgramUniform2fv(programId, scaleLoc, 1, glm::value_ptr(scale));
#else
		program.uniform(colorLoc, color);
		program.uniform(positionLoc, position);
		program.uniform(scaleLoc, scale);
#endif
		fmt::print("Section 1 error: {}\n", rt::getLastErrorString());

		using index_t = uint32_t;

		// SECTION 2:
#ifndef USE_RENDER_TOOLS
		glEnableVertexArrayAttrib(vaoId, 0);
		glVertexArrayAttribBinding(vaoId, 0, 0);
#else
		vao.attribEnable(0);
		vao.attribBinding(0, 0);
#endif
		fmt::print("Section 2 error: {}\n", rt::getLastErrorString());

		// SECTION 3:
#ifndef USE_RENDER_TOOLS
		glBindVertexArray(vaoId);
		switch (sizeof(index_t)) {
		case 1:
			glVertexArrayAttribIFormat(vaoId, 0, 1, GL_UNSIGNED_BYTE, 0);
			break;
		case 2:
			glVertexArrayAttribIFormat(vaoId, 0, 1, GL_UNSIGNED_SHORT, 0);
			break;
		case 4:
			glVertexArrayAttribIFormat(vaoId, 0, 1, GL_UNSIGNED_INT, 0);
			break;
		}
		glBindVertexArray(0);
#else
		// Bind because intel drivers suck.
		vao.bind();
		switch (sizeof(index_t)) {
		case 1:
			vao.attribFormatI32(0, 1, 0, rt::Type::U8);
			break;
		case 2:
			vao.attribFormatI32(0, 1, 0, rt::Type::U16);
			break;
		case 4:
			vao.attribFormatI32(0, 1, 0, rt::Type::U32);
			break;
		}
		vao.unbind();
#endif
		fmt::print("Section 3 error: {}\n", rt::getLastErrorString());

		index_t indices[]{
			0, 2, 1, 2, 3, 1
		};

		// SECTION 4:
#ifndef USE_RENDER_TOOLS
		glNamedBufferStorage(vbId, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayVertexBuffer(vaoId, 0, vbId, 0, sizeof(index_t));

		glUseProgram(programId);
		glBindVertexArray(vaoId);
#else
		vbo.initArray(indices, sizeof(indices) / sizeof(index_t), rt::BufferInit::Dynamic);
		vao.bindVertex(vbo, 0, 0, sizeof(index_t));
		
		program.bind();
		vao.bind();
#endif
		fmt::print("Section 4 error: {}\n", rt::getLastErrorString());

		// SECTION 5:
#ifndef USE_RENDER_TOOLS
		int dim = 0, stride = 0, type = 0, isInt = 0;
		glGetVertexArrayIndexediv(vaoId, 0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &dim);
		glGetVertexArrayIndexediv(vaoId, 0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
		glGetVertexArrayIndexediv(vaoId, 0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
		glGetVertexArrayIndexediv(vaoId, 0, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &isInt);
#else
		int dim = 0, stride = 0;
		GLenum type = 0;
		bool isInt = false;

		dim = vao.getAttribDim(0);
		stride = vao.getAttribStride(0);
		type = vao.getAttribType(0);
		isInt = vao.isAttribInteger(0);
#endif
		fmt::print("Section 5 error: {}\n", rt::getLastErrorString());
		fmt::print("Dim: {}, Stride: {}, Type: {}, isInt: {}\n", dim, stride, type, isInt);

		sf::Event ev;
		bool running = true;
		while (running) {
			while (window.pollEvent(ev)) {
				if (ev.type == sf::Event::Closed) {
					running = false;
					break;
				}
			}

			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(indices) / sizeof(index_t));

			glFinish();

			window.display();
		}

		// SECTION 6:
#ifndef USE_RENDER_TOOLS
		glBindVertexArray(0);
		glUseProgram(0);

		glDeleteShader(fragShader);
		glDeleteShader(vertShader);

		glDeleteProgram(programId);
		glDeleteBuffers(1, &vbId);
		glDeleteVertexArrays(1, &vaoId);
#else
		vao.unbind();
		program.unbind();
#endif
		fmt::print("Section 6 error: {}\n", rt::getLastErrorString());
	}
}