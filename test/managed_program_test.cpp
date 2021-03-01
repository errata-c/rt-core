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

		ColoredQuadProgram program;
		
		program.bind();

		while (loopWindow(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
			program.draw();

			window->display();
		}

		program.unbind();
	}
	cleanup(window);

	return 0;
}