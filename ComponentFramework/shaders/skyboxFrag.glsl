#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
	//fragColor = texture(skybox, texCoords)
}