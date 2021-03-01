#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <Utilities.hpp>

#include <vector>

#include <rt/Program.hpp>
#include <rt/VertexArray.hpp>
#include <rt/Buffer.hpp>
#include <rt/Texture.hpp>
#include <rt/Fence.hpp>

#include <rt/GLError.hpp>


int main() {
	sf::Window* window = initializeWindow();
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		struct Vert {
			glm::vec2 pos;
			glm::vec3 color;
		};
		rt::CoherentBuffer vertices(6 * sizeof(Vert));
		assert(vertices.isValid());

		Vert* vptr = vertices.map<Vert>();
		
		if (vptr != nullptr) {
			vptr[0] = { glm::vec2{-1, +1}, glm::vec3{1, 0, 0} };
			vptr[1] = { glm::vec2{-1, -1}, glm::vec3{0, 1, 0} };
			vptr[2] = { glm::vec2{+1, +1}, glm::vec3{0, 0, 1} };

			vptr[3] = { glm::vec2{-1, -1}, glm::vec3{0, 1, 0} };
			vptr[4] = { glm::vec2{+1, -1}, glm::vec3{1, 0, 1} };
			vptr[5] = { glm::vec2{+1, +1}, glm::vec3{0, 0, 1} };
		}
		else {
			throw std::exception("No pointer returned when mapping vertices buffer.");
		}

		rt::VertexArray vao;
		vao.bind();
		vao.attribFormatF32(0, 2, offsetof(Vert, pos));
		vao.attribFormatF32(1, 3, offsetof(Vert, color));
		vao.attribEnable(0);
		vao.attribEnable(1);
		vao.attribBinding(0, 0);
		vao.attribBinding(1, 0);
		vao.bindVertex(vertices, 0, 0, sizeof(Vert));
		vao.unbind();

		glm::mat4 modelMat = glm::mat4{ 1 };

		const char* vertShader =
			"#version 450\n"
			"layout(location = 0) in vec2 pos;\n"
			"layout(location = 1) in vec3 color;\n"
			"out vec3 fragColor;\n"
			"uniform mat4 model;\n"
			"void main() {\n"
			"gl_Position = model * vec4(pos, 0, 1);\n"
			"fragColor = color;\n"
			"}\n";

		const char* fragShader =
			"#version 450\n"
			"in vec3 fragColor;"
			"out vec4 finalColor;\n"
			"void main() {\n"
			"finalColor = vec4(fragColor, 1);\n"
			"}\n";

		rt::Program program;
		program.compile(vertShader, fragShader);
		GLuint modelIndex = program.getUniformLocation("model");
		program.uniform(modelIndex, modelMat);

		rt::printLastError();

		GLuint vertBufferId = vao.getAttribVertexBufferId(0);
		fmt::print("Vertex Binding == {}\n", vertBufferId);
		fmt::print("Actual Buffer == {}\n", vertices.getId());
		
		rt::printLastError();

		program.bind();
		vao.bind();

		while (loopWindow(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			vao.drawArrays(rt::Primitive::Triangles, 6);

			window->display();
		}

		vao.unbind();
		program.unbind();
	}
	cleanup(window);

	return 0;
}