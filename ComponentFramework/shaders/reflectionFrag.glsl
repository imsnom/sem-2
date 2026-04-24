#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 fragColor;
layout(location = 1) in vec3 normal;
//in vec3 pos;

layout(location = 2) in vec3 uvwCoord;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {

	vec3 i = normalize(uvwCoord - cameraPos);
	vec3 r = reflect(i, normalize(normal));
	fragColor = vec4(texture(skybox, r).rgb, 1.0);
}