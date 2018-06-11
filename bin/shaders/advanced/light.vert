// a simple shader 
#version 410 
 
layout( location = 0 ) in vec4 Position;

out vec4 position;
out vec4 vTexCoord;

uniform mat4 ProjectionViewModel;
uniform float radius;
 
void main() 
{  
	vec4 p = Position;
	p.xyz *= radius * 2.0f;
	
	gl_Position = ProjectionViewModel * p;
}