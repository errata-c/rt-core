# rt-core
Opengl 4.5 utilities

Provides a number of classes for opengl types, using the direct state access api of OpenGL 4.5

### CMake
```cmake
find_package(rt-core CONFIG REQUIRED)

target_link_libraries(foo PRIVATE rt::core)
```


### C++

```cpp
#include <rt/rt.hpp>

int main(int argc, char* argv[]) {
	rt::load();

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
	vao.attribFormatF32(0, 2, offsetof(Vert, pos));
	vao.attribFormatF32(1, 3, offsetof(Vert, color));
	vao.attribEnable(0);
	vao.attribEnable(1);
	vao.attribBinding(0, 0);
	vao.attribBinding(1, 0);
	vao.bindVertex(vertices, 0, 0, sizeof(Vert));
	
	return 0;
}
```
