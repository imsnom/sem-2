#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPos;

out vec2 uvCoordFromVert;
out vec3 normalFromVert;
out vec3 vertPosFromVert;
out vec3 eyeDirFromVert;
out vec3 lightDirFromVert;

void main() {
    uvCoordFromVert = uvCoord;
    normalFromVert = vNormal;

    vertPosFromVert = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPosFromVert);
    eyeDirFromVert = -vertDir;
    lightDirFromVert = normalize(vec3(lightPos) - vertPosFromVert);


    gl_Position =  vVertex;
}
