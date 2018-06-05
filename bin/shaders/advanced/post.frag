//post processing fragment
#version 410 
 
in vec2 vTexCoord;

uniform sampler2D displayTexture;

out vec4 FragColour; 
 
void main() 
{
	//texel size
	vec2 texSize = textureSize(displayTexture, 0);
	vec2 texelSize = 1.0 / texSize;
	
	//adust texture coordinate by a half texel, sample from middle
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	FragColour = texture(displayTexture, texCoord);
}