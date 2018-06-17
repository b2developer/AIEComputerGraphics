//post processing fragment
#version 410 
 
in vec2 vTexCoord;

uniform sampler2D displayTexture;
uniform float exposure = 1.5f;

out vec4 FragColour; 
 
void main() 
{
	//texel size
	vec2 texSize = textureSize(displayTexture, 0);
	vec2 texelSize = 1.0 / texSize;
	
	//adust texture coordinate by a half texel, sample from middle
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;
	
	vec3 colour = texture(displayTexture, texCoord).rgb;
	colour *= exposure;
	
	vec3 x = vec3(max(0, colour.x - 0.004f), max(0, colour.y - 0.004f), max(0, colour.z - 0.004f));
	colour = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);
	
	FragColour = vec4(colour, 1.0f);
}