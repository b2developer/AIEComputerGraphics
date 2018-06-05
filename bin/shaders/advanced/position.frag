//gets the colour of the texture
#version 410 
 
in vec4 position;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColour; 
 
void main() 
{
	vec3 sposition = position.xyz;
	
	FragColour = vec4(sposition, 1);
}