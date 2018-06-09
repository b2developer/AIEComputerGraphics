//phong shader
#version 410 
 
in vec2 vTexCoord;

//informational buffers from G and L passes
uniform sampler2D albedoTexture;
uniform sampler2D lightTexture;
uniform sampler2D specularTexture;

out vec4 FragColour;

void main() 
{
	vec3 albedo = texture(albedoTexture, vTexCoord).rgb;
	vec3 light = texture(lightTexture, vTexCoord).rgb;
	vec3 specular = texture(specularTexture, vTexCoord).rgb;
	
	FragColour = vec4(albedo * light + specular, 1.0f);
}

