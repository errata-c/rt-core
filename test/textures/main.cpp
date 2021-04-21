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
			glm::vec2 uv;
		};
		std::array<Vert, 6> varr{
			Vert{ glm::vec2{-1, +1}, glm::vec2{0, 0} },
			Vert{ glm::vec2{-1, -1}, glm::vec2{0, 1} },
			Vert{ glm::vec2{+1, +1}, glm::vec2{1, 0} },

			Vert{ glm::vec2{-1, -1}, glm::vec2{0, 1} },
			Vert{ glm::vec2{+1, -1}, glm::vec2{1, 1} },
			Vert{ glm::vec2{+1, +1}, glm::vec2{1, 0} },
		};

		rt::ImmutableBuffer vertices(varr.data(), varr.size());
		assert(vertices.isValid());

		rt::VertexArray vao;
		vao.attribFormatF32(0, 2, offsetof(Vert, pos));
		vao.attribFormatF32(1, 2, offsetof(Vert, uv));
		vao.attribEnable(0);
		vao.attribEnable(1);
		vao.attribBinding(0, 0);
		vao.attribBinding(1, 0);
		vao.bindVertex(vertices, 0, 0, sizeof(Vert));

		glm::mat4 modelMat = glm::mat4{ 1 };

		std::string vertShader = loadSource("texture.vs.glsl");
		std::string fragShader = loadSource("texture.fs.glsl");

		rt::Program program;
		program.compile(vertShader, fragShader);

		GLuint modelIndex = program.getUniformLocation("model");
		GLuint texIndex = program.getUniformLocation("tex");

		program.uniform(modelIndex, modelMat);
		program.uniformSampler(texIndex, 0);

		rt::ImmutableTexture2d tex0, tex1, tex2;

		{
			uint8_t 
				b = 0, 
				y = 0b111'110'01; // FFEB3B Hex, mapped to 8 bits it is almost yellow...

			rt::PixelFormat form = rt::PixelFormat::U8_3_3_2;
			rt::PixelComponent comp = rt::PixelComponent::RGB;

			std::array<uint8_t, 64> packedByte{
				y,y,y,y, y,y,y,y,
				y,b,b,y, y,b,b,y,
				y,b,b,y, y,b,b,y,
				y,y,y,y, y,y,y,y,

				y,y,y,y, y,y,y,y,
				y,b,y,y, y,y,b,y,
				y,b,b,b, b,b,b,y,
				y,y,y,y, y,y,y,y,
			};

			tex0.init(rt::TexFormat::RGB_N8, 1, {8, 8});
			tex0.subImage2D(packedByte.data(), 0, glm::ivec2{ 0 }, glm::ivec2{ 8,8 }, comp, form);
			tex0.filterNearest();
		}
		{
			uint16_t
				b = 0,
				y = 0xFE3F; // FFEB3B Hex, mapped to 16 bits its much closer to original.

			// Byte swap is not necessary with hex literal, happens automatically.
			//y = ((0xFF00 & y) >> 8) | ((0x00FF & y) << 8);
			

			rt::PixelFormat form = rt::PixelFormat::U16_4_4_4_4;
			rt::PixelComponent comp = rt::PixelComponent::RGBA;

			std::array<uint16_t, 64> packedShort{
				y,y,y,y, y,y,y,y,
				y,b,b,y, y,b,b,y,
				y,b,b,y, y,b,b,y,
				y,y,y,y, y,y,y,y,

				y,y,y,y, y,y,y,y,
				y,b,y,y, y,y,b,y,
				y,b,b,b, b,b,b,y,
				y,y,y,y, y,y,y,y,
			};

			tex1.init(rt::TexFormat::RGBA_N8, 1, { 8, 8 });
			tex1.subImage2D(packedShort.data(), 0, glm::ivec2{ 0 }, glm::ivec2{ 8, 8 }, comp, form);
			tex1.filterNearest();
		}
		{
			uint16_t
				b = 0,
				y = 0b11111'111010'00111; // FFEB3B Hex

			// Byte swap IS necessary for binary literals.
			y = ((0xFF00 & y) >> 8) | ((0x00FF & y) << 8);

			rt::PixelFormat form = rt::PixelFormat::RU16_5_6_5;
			rt::PixelComponent comp = rt::PixelComponent::RGB;

			std::array<uint16_t, 64> packedShort{
				y,y,y,y, y,y,y,y,
				y,b,b,y, y,b,b,y,
				y,b,b,y, y,b,b,y,
				y,y,y,y, y,y,y,y,

				y,y,y,y, y,y,y,y,
				y,b,y,y, y,y,b,y,
				y,b,b,b, b,b,b,y,
				y,y,y,y, y,y,y,y,
			};

			tex2.init(rt::TexFormat::RGB_N8, 1, { 8, 8 });
			tex2.subImage2D(packedShort.data(), 0, glm::ivec2{ 0 }, glm::ivec2{ 8, 8 }, comp, form);
			tex2.filterNearest();
		}

		program.bind();
		vao.bind();

		// Switch this to try out the other textures!
		tex2.bindUnit(0);

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