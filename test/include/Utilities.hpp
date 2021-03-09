#pragma once
#include <thread>
#include <chrono>
#include <fstream>

#include <string>
#include <string_view>

#include <filesystem>

#include <fmt/format.h>

#include <rt/Program.hpp>
#include <rt/Texture.hpp>
#include <rt/VertexArray.hpp>

#include <SFML/Window.hpp>

#include "config.hpp"

std::string loadSource(std::string_view filename) {
	namespace fs = std::filesystem;
	fs::path path = fs::path(resources_directory) / filename;

	std::ifstream file(path);
	std::streampos start = file.tellg();
	std::streampos end = file.seekg(0, std::ios::end).tellg();
	file.seekg(0, std::ios::beg);
	std::size_t length = end - start;

	std::string text;
	text.resize(length);
	file.read(text.data(), length);
	return text;
}

sf::Window * initializeWindow() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 5;

	sf::Window * window = new sf::Window(sf::VideoMode(800, 600), "rt test", sf::Style::Default, settings);

	settings = window->getSettings();

	fmt::print("Depth bits: {}\n", settings.depthBits);
	fmt::print("Version: {}.{}\n", settings.majorVersion, settings.minorVersion);

	fmt::print("Max array texture layers: {}\n", GL_MAX_ARRAY_TEXTURE_LAYERS);

	window->setActive(true);
	fmt::print("Window made active.\n");

	if (!rt::load()) {
		throw std::exception("Failed to initialize rt!");
	}

	return window;
}

void cleanup(sf::Window* window) {
	window->close();
	delete window;
}

bool loopWindow(sf::Window* window) {
	// check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;
	while (window->pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed) {
			return false;
		}
		else if (event.type == sf::Event::Resized) {
			auto size = window->getSize();
			glViewport(0, 0, size.x, size.y);
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	return true;
}

class ColoredQuadProgram {
public:
	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;
	};
	static constexpr std::array<Vertex, 6> verts{
		Vertex{ glm::vec2{-1, +1}, glm::vec3{1, 0, 0} },
		Vertex{ glm::vec2{-1, -1}, glm::vec3{0, 1, 0} },
		Vertex{ glm::vec2{+1, +1}, glm::vec3{0, 0, 1} },

		Vertex{ glm::vec2{-1, -1}, glm::vec3{0, 1, 0} },
		Vertex{ glm::vec2{+1, -1}, glm::vec3{1, 0, 1} },
		Vertex{ glm::vec2{+1, +1}, glm::vec3{0, 0, 1} },
	};

	ColoredQuadProgram()
		: vertices(sizeof(verts), rt::BufferInit::Persistent | rt::BufferInit::Coherent | rt::BufferInit::Write)
	{
		assert(vertices.isValid());
		{
			void* vptr = vertices.map(rt::BufferFlag::Persistent | rt::BufferFlag::Coherent | rt::BufferFlag::Write);
			assert(vptr != nullptr);
			std::memcpy(vptr, verts.data(), sizeof(verts));
			vertices.unmap();
		}

		vao.attribFormatF32(0, 2, offsetof(Vertex, pos));
		vao.attribFormatF32(1, 3, offsetof(Vertex, color));
		vao.attribEnable(0);
		vao.attribEnable(1);
		vao.attribBinding(0, 0);
		vao.attribBinding(1, 0);
		vao.bindVertex(vertices, 0, 0, sizeof(Vertex));

		std::string vertShader = loadSource("basic.vs.glsl");
		std::string fragShader = loadSource("basic.fs.glsl");

		program.compile(vertShader, fragShader);
		auto tmp0 = program.getInputLocation("pos"),
			tmp1 = program.getInputLocation("color");

		uModel = program.getUniformLocation("model");

		glm::mat4 modelMat = glm::mat4{ 1 };
		program.uniform(uModel, modelMat);
	}

	void bind() {
		program.bind();
		vao.bind();
	}
	void draw() {
		vao.drawArrays(rt::Primitive::Triangles, 6);
	}
	void unbind() {
		vao.unbind();
		program.unbind();
	}

	GLint uModel;
	rt::ImmutableBuffer vertices;
	rt::VertexArray vao;
	rt::Program program;
};

class TexturedQuadProgram {
public:
	struct Vertex {
		glm::vec2 pos;
		glm::vec2 uv;
	};
	static constexpr std::array<Vertex, 6> verts{
		Vertex{ glm::vec2{-1, +1}, glm::vec2{0, 1} },
		Vertex{ glm::vec2{+1, +1}, glm::vec2{1, 1} },
		Vertex{ glm::vec2{-1, -1}, glm::vec2{0, 0} },

		Vertex{ glm::vec2{-1, -1}, glm::vec2{0, 0} },
		Vertex{ glm::vec2{+1, -1}, glm::vec2{1, 0} },
		Vertex{ glm::vec2{+1, +1}, glm::vec2{1, 1} },
	};

	TexturedQuadProgram() 
		: vertices(sizeof(verts), rt::BufferInit::Persistent | rt::BufferInit::Coherent | rt::BufferInit::Write)
	{
		assert(vertices.isValid());
		{
			void* vptr = vertices.map(rt::BufferFlag::Persistent | rt::BufferFlag::Coherent | rt::BufferFlag::Write);
			assert(vptr != nullptr);
			std::memcpy(vptr, verts.data(), sizeof(verts));
		}
	
		vao.attribFormatF32(0, 2, offsetof(Vertex, pos));
		vao.attribFormatF32(1, 2, offsetof(Vertex, uv));
		vao.attribEnable(0);
		vao.attribEnable(1);
		vao.attribBinding(0, 0);
		vao.attribBinding(1, 0);
		vao.bindVertex(vertices, 0, 0, sizeof(Vertex));

		std::string vertShader = loadSource("texture.vs.glsl");
		std::string fragShader = loadSource("texture.fs.glsl");

		program.compile(vertShader, fragShader);
		uModel = program.getUniformLocation("model");
		uTex = program.getUniformLocation("tex");

		glm::mat4 modelMat = glm::mat4{ 1 };
		program.uniform(uModel, modelMat);
	}

	void setTexUnit(GLint unit) {
		program.uniformSampler(uTex, &unit, 1);
	}

	void bind() {
		program.bind();
		vao.bind();
	}
	void draw() {
		vao.drawArrays(rt::Primitive::Triangles, 6);
	}
	void unbind() {
		vao.unbind();
		program.unbind();
	}

	GLint uModel, uTex;
	rt::ImmutableBuffer vertices;
	rt::VertexArray vao;
	rt::Program program;
};