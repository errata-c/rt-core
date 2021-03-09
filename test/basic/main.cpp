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
	using Flags = rt::BufferFlags;

	sf::Window* window = initializeWindow();
	{
		rt::printLastError();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rt::printLastError();

		struct Vert {
			glm::vec2 pos;
			glm::vec3 color;
		};
		std::array<Vert, 6> varr{
			Vert{ glm::vec2{-1, +1}, glm::vec3{1, 0, 0} },
			Vert{ glm::vec2{-1, -1}, glm::vec3{0, 1, 0} },
			Vert{ glm::vec2{+1, +1}, glm::vec3{0, 0, 1} },

			Vert{ glm::vec2{-1, -1}, glm::vec3{0, 1, 0} },
			Vert{ glm::vec2{+1, -1}, glm::vec3{1, 0, 1} },
			Vert{ glm::vec2{+1, +1}, glm::vec3{0, 0, 1} },
		};

		rt::ImmutableBuffer vertices(varr.data(), varr.size());
		assert(vertices.isValid());

		rt::VertexArray vao;
		vao.attribFormatF32(0, 2, offsetof(Vert, pos));
		vao.attribFormatF32(1, 3, offsetof(Vert, color));
		vao.attribEnable(0);
		vao.attribEnable(1);
		vao.attribBinding(0, 0);
		vao.attribBinding(1, 0);
		vao.bindVertex(vertices, 0, 0, sizeof(Vert));

		glm::mat4 modelMat = glm::mat4{ 1 };

		std::string vertShader = loadSource("basic.vs.glsl");
		std::string fragShader = loadSource("basic.fs.glsl");

		rt::printLastError();

		rt::Program program;
		program.compile(vertShader, fragShader);
		rt::printLastError();

		{
			GLint numUniforms = 0;
			glGetProgramInterfaceiv(program.getId(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
			const GLenum properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
			for (int i = 0; i < numUniforms; ++i) {
				GLint values[4];
				glGetProgramResourceiv(program.getId(), GL_UNIFORM, i, 4, properties, 4, NULL, values);

				std::string nameData(values[2], '\0');
				glGetProgramResourceName(program.getId(), GL_UNIFORM, i, nameData.size(), NULL, nameData.data());

				std::cout << "name == " << nameData << "\n";
				std::cout << "type == " << values[1] << "\n";
			}
		}

		GLuint modelIndex = program.getUniformLocation("model");
		rt::printLastError();

		program.uniform(modelIndex, modelMat);

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