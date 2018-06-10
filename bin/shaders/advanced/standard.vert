// a simple shader 
#version 410 
 
layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec3 Normal;
layout( location = 2 ) in vec2 TexCoord;
layout( location = 3 ) in vec4 Tangent;

out vec4 position;
out vec3 vNormal;

//which way the texture spans (tangent = x, bi = y)
out vec3 vTangent;
out vec3 vBiTangent;

out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
 
void main() 
{  
	vNormal = NormalMatrix * Normal.xyz;
	vTexCoord = TexCoord;
	position = ModelMatrix * Position;
	
	vTangent = NormalMatrix * Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
	
	gl_Position = ProjectionViewModel * Position;
}