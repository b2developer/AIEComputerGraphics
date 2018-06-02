// a simple flat colour shader 
#version 410 
 
//diffuse
uniform vec3 Kd; 

out vec4 FragColour; 
 
void main() 
{  
	FragColour = vec4(Kd, 1);
}