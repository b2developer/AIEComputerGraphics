// a simple flat colour shader 
#version 410 
 
//diffuse
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