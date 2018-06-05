//untransformed shader
#version 410
 
layout( location = 0 ) in vec2 Position;

out vec2 vTexCoord;

uniform vec2 position;
uniform vec2 scale;
uniform float depth;

void main() 
{  
	vTexCoord = (Position * 0.5f + 0.5f);
	gl_Position = vec4(Position * scale + position, depth, 1);
}