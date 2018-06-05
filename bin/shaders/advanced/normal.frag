//gets the colour of the texture
#version 410 
 
in vec4 position;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

out vec4 FragColour; 

uniform int useTexture = 0;
uniform sampler2D normalTexture;
 
void main() 
{
	vec3 N = normalize(vNormal);
	
	int iut = 1 - useTexture;

	if (useTexture == 0)
	{
		FragColour = vec4(N, 1);
	}
	else
	{
		vec3 T = normalize(vTangent);
		vec3 B = normalize(vBiTangent);
		
		//tangent basis matrix, rotation matrix to the plane of the tangent
		mat3 TBN = mat3(T, B, N);
		
		vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
	
		N = TBN * (texNormal * 2 - 1);
		
		FragColour = vec4(N, 1);
	}
}