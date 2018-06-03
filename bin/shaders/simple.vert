// a simple shader 
#version 410 
 
layout( location = 0 ) in vec4 Position; 
layout( location = 2 ) in vec2 TexCoord;

out vec2 vTexCoord;
uniform mat4 ProjectionViewModel; 
 
void main() 
{  
	vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position; 
}