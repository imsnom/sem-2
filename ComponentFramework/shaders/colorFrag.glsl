#version 450
#extension GL_ARB_separate_shader_objects : enable

// color sent from CPU
layout(location = 4) uniform vec4 color;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = color;
}