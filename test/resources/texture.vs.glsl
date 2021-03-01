#version 450
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
out vec2 fragUV;
uniform mat4 model;

void main() {
	gl_Position = model * vec4(pos, 0, 1);
	fragUV = uv;
}