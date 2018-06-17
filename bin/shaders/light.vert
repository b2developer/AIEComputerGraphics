// a simple shader 
#version 410 
 
layout( location = 0 ) in vec4 Position;

out vec4 position;
out vec4 vTexCoord;

uniform mat4 ProjectionViewModel;
uniform vec3 scale;
 
void main() 
{  
	vec4 p = Position;
	p.xyz *= scale;
	
	gl_Position = ProjectionViewModel * p;
}