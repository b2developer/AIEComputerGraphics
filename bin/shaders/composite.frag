//composite shader
#version 410 
 
in vec2 vTexCoord;

//informational buffers from G, L and T passes
uniform sampler2D ambientTexture;
uniform sampler2D albedoTexture;
uniform sampler2D lightTexture;
uniform sampler2D specularTexture;
uniform sampler2D transparentTexture;
uniform sampler2D blendTexture;

out vec4 FragColour;

void main() 
{
	vec3 ambient = texture(ambientTexture, vTexCoord).rgb;
	vec3 albedo = texture(albedoTexture, vTexCoord).rgb;
	vec3 light = texture(lightTexture, vTexCoord).rgb;
	vec3 specular = texture(specularTexture, vTexCoord).rgb;
	vec4 trans = texture(transparentTexture, vTexCoord).rgba;
	float blend = texture(blendTexture, vTexCoord).r;
	
	float transAlpha = blend;
	float otherAlpha = 1 - transAlpha;
	
	
	FragColour = vec4((ambient + albedo * light + specular) * otherAlpha + trans.rgb * transAlpha, 1.0f);
}

