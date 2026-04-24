#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles, equal_spacing, ccw) in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform sampler2D textureData;

layout (binding = 0) uniform sampler2D heightTextureMap;
layout (binding = 1) uniform sampler2D normalTextureMap;

in vec2 uvCoordFromCtrl[];
in vec3 normalFromCtrl[];
in vec3 vertPosFromCtrl[];
in vec3 eyeDirFromCtrl[];
in vec3 lightDirFromCtrl[];


out vec2 uvCoordFromEval;
out vec3 normalFromEval;
out vec3 vertPosFromEval;
out vec3 eyeDirFromEval;
out vec3 lightDirFromEval;

vec2 interpolateVec2(vec2 v0, vec2 v1, vec2 v2) {
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec3 interpolateVec3(vec3 v0, vec3 v1, vec3 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec4 interpolateVec4(vec4 v0, vec4 v1, vec4 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

void main() {
   
    uvCoordFromEval = interpolateVec2(uvCoordFromCtrl[0], uvCoordFromCtrl[1], uvCoordFromCtrl[2]);
    //normalFromEval =  interpolateVec3(normalFromCtrl[0], normalFromCtrl[1], normalFromCtrl[2]);

    vec3 normalSample = texture(normalTextureMap, uvCoordFromEval).rgb;
    normalFromEval = normalize(normalSample * 2.0f - 1.0f);

    vertPosFromEval = interpolateVec3(vertPosFromCtrl[0], vertPosFromCtrl[1], vertPosFromCtrl[2]);
    eyeDirFromEval = interpolateVec3(eyeDirFromCtrl[0], eyeDirFromCtrl[1], eyeDirFromCtrl[2]);
    lightDirFromEval = interpolateVec3(lightDirFromCtrl[0], lightDirFromCtrl[1], lightDirFromCtrl[2]);

    /// Interpolate position across the triangle
    vec4 position = interpolateVec4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

    vec4 height = vec4(texture(heightTextureMap, uvCoordFromEval));
    position.z = height.r;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}