#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;
uniform vec3 lightPos_2;
//uniform vec3 lightPosMulti[2] = {lightPos, lightPos_2};

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir; 
layout(location = 3) out vec2 textCoord;
layout(location = 4) out vec3 lightDir_2;


void main() {
    textCoord = uvCoord;
    textCoord.y *= -1;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    lightDir = normalize(vec3(lightPos) - vertPos); 
    lightDir_2 = normalize(vec3(lightPos_2) - vertPos);
	
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
    
}
/// vVertex is in Model space 
	/// modelMatrix * vVertex is in World space
	/// viewMatrix * modelMatrix * vVertex is in Camera space 
	/// projectionMatrix * viewMatrix * modelMatrix * vVertex is in NDC space 
	/// I'll explain spaces in class. 