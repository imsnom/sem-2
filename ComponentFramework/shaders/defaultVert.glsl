#version 450
#extension GL_ARB_separate_shader_objects : enable

// attributes 
layout(location = 0) in vec3 inVertex; 
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUVCoord;

// uniforms
layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;
layout(location = 3) uniform vec3 lightPos;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inVertex, 1.0);
}