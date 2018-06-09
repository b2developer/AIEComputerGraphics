//phong shader
#version 410 
 
in vec2 vTexCoord;

//informational buffers from G and L passes
uniform sampler2D albedoTexture;
uniform sampler2D lightTexture;

out vec4 FragColour;

void main() 
{
	vec3 albedo = texture(albedoTexture, vTexCoord).rgb;
	vec3 light = texture(lightTexture, vTexCoord).rgb;
	
	FragColour = vec4(albedo * light, 1.0f);
}

