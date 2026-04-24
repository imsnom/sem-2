#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 uvCoordFromEval;
in vec3 normalFromEval;
in vec3 vertPosFromEval;
in vec3 eyeDirFromEval;
in vec3 lightDirFromEval;

uniform float density;

out vec4 fragColor;

layout (binding = 0) uniform sampler2D heightTextureMap;
layout (binding = 2) uniform sampler2D diffuseTextureMap;

uniform vec3 cameraPos;
uniform sampler2D textureData;


void main() {

    vec4 ks = vec4(0.9, 0.9, 0.9, 0.0);
    vec4 kd = vec4(0.8, 0.8, 0.8, 0.0);
    vec4 ka = 0.1 * kd;
    vec4 kt = texture(diffuseTextureMap, uvCoordFromEval);
    float diff = dot(normalFromEval, lightDirFromEval);

    vec3 reflection = normalize(reflect(-lightDirFromEval, normalFromEval));

    float spec = max(dot(eyeDirFromEval, reflection), 0.0);

    spec = pow(spec, 14.0f);

    vec3 originalColor = vec3((ka + (diff * kd) + (spec * ks))) * kt.xyz;
    float distance = distance(vertPosFromEval, cameraPos);
    float fogAmount = 1.0 - exp(-distance * density);
    vec3 fogColor = vec3(0.26f, 0.28f, 0.58f);
	vec3 finalColor = mix(originalColor, fogColor, fogAmount);

    fragColor = vec4(finalColor, 1.0f);
    ///fragColor = texture(textureData,uvCoordFromEval);
    //fragColor = vec4(1.0,1.0,1.0,1.0);
}