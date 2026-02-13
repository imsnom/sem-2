#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

uniform sampler2D theTexture;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textCoord;
layout(location = 4) in vec3 lightDir_2;

void main() {
    vec4 ks = vec4(0.3, 0.3, 0.9, 0.0); // colour
	vec4 ks_2 = vec4 (0.9, 0.3, 0.3, 0.0);
	vec4 kd = vec4(0.7, 0.7, 0.7, 0.0); // brightness
	vec4 kd_2 = vec4(0.7, 0.7, 0.7, 0.0);
	vec4 ka = 0.1 * (kd + kd_2);

	vec4 textureColor = texture(theTexture, textCoord);

	float diff = max(dot(vertNormal, lightDir), 0.0);
	float diff_2 = max(dot(vertNormal, lightDir_2), 0.0f);

	/// Reflection is based incedent direction which means a vector from the light source
	/// not the direction to the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	vec3 reflection_2 = normalize(reflect(-lightDir_2, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	float spec_2 = max(dot(eyeDir, reflection_2), 0.0);
	spec = pow(spec, 14.0);
	spec_2 = pow(spec_2, 14.0);
	//fragColor =  (ka + ((diff + diff_2) * (kd + kd_2)) + ((spec * spec_2) * (ks + ks_2))) * textureColor;
	//fragColor = (ka + (((diff + kd) * (diff_2 + kd_2)) + ((spec + ks) * (spec_2 + ks_2)))) * textureColor;
	fragColor = (ka + ((diff + kd) * (spec + ks)) * ((diff_2 + kd_2) * (spec_2 + ks_2))) * textureColor;
	//fragColor =  ka + (diff * kd) + (spec * ks);	
	//fragColor = (ka + ((diff + diff_2) * kd) + ((spec + spec_2) * ks)) * textureColor;	
	//fragColor = (ka + ((diff + diff_2) * (kd * kd_2)) + ((spec + spec_2) * (ks * ks_2))) * textureColor;

}