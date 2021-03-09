#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <Utilities.hpp>

#include <vector>
#include <rt/rt.hpp>

int main() {
	sf::Window* window = initializeWindow();
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ColoredQuadProgram program;
		TexturedQuadProgram texProgram;

		rt::printLastError();

		rt::FrameBuffer fb;
		rt::ImmutableTexture2d rbColor;
		rt::RenderBuffer rbDepthStencil;

		rbColor.init(rt::TexFormat::RGBA_N8, 1, { 800, 600 });
		rt::printLastError();

		rbDepthStencil.init(rt::TexFormat::D24_S8, { 800, 600 });
		rt::printLastError();

		fb.attachColor(rbColor, 0);
		rt::printLastError();

		fb.attach(rbDepthStencil, rt::FBAttach::DepthStencil);
		rt::printLastError();

		fb.bind();

		program.bind();
		program.draw();
		program.unbind();

		fb.unbind();

		rbColor.bindUnit(0);
		texProgram.setTexUnit(0);

		glm::ivec2 frame{ 800, 600 };

		texProgram.bind();
		while (loopWindow(window))
		{
			glm::ivec2 size{ window->getSize().x, window->getSize().y };
			if (size.x != frame.x || frame.y != size.y) {
				frame = size;

				rbColor.reset();
				rbDepthStencil.reset();

				rbColor.init(rt::TexFormat::RGBA_N8, 1, frame);
				rbDepthStencil.init(rt::TexFormat::D24_S8, frame);

				fb.attachColor(rbColor, 0);
				fb.attach(rbDepthStencil, rt::FBAttach::DepthStencil);

				fb.bind();
				glClear(GL_COLOR_BUFFER_BIT);

				rt::FBStatus status = fb.status();
				switch (status) {
				case rt::FBStatus::IncompleteAttachment:
					fmt::print("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
					break;
				case rt::FBStatus::Complete:
					fmt::print("GL_FRAMEBUFFER_COMPLETE\n");
					break;
				}

				program.bind();
				program.draw();
				program.unbind();
				fb.unbind();

				texProgram.bind();
				rbColor.bindUnit(0);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			texProgram.draw();

			window->display();
		}
		texProgram.unbind();

	}
	cleanup(window);

	return 0;
}