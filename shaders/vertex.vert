#version 330 core

#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
}
