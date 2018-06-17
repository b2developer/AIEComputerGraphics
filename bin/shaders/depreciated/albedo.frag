//gets the colour of the texture
#version 410 
 
in vec4 position;
in vec3 vNormal;
in vec2 vTexCoord;

uniform int useTexture = 1;
uniform vec3 Kd;
uniform sampler2D diffuseTexture;

out vec4 FragColour; 
 
void main() 
{  
	int inT = 1 - useTexture;

	FragColour = (texture(diffuseTexture, vTexCoord) + vec4(inT, inT, inT, 0.0)) * vec4(Kd, 1.0);
}