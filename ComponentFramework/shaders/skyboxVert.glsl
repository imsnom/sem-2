#version 450
#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec3 vertex;


out vec3 texCoords;

uniform mat4 projection;
uniform mat4 model;

void main() {
	texCoords = vec3(vertex);
	gl_Position = projection * model * vec4(vertex, 1.0);
