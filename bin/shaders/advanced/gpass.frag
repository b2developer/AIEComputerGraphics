//gets the colour of the texture
#version 410 
 
in vec4 position;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

uniform int useTexture = 0;
uniform int useNormalTexture = 0;
uniform vec3 Kd;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

layout(location = 0) out vec4 albedoC;
layout(location = 1) out vec4 positionC;
layout(location = 2) out vec4 normalC;

void main() 
{
	int inT = 1 - useTexture;

	albedoC = (texture(diffuseTexture, vTexCoord) + vec4(inT, inT, inT, 0.0)) * vec4(Kd, 1.0);

	vec3 sposition = position.xyz;
	
	positionC = vec4(sposition, 1);

	vec3 N = normalize(vNormal);

	if (useNormalTexture == 0)
	{
		normalC = vec4(N * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1);
	}
	else
	{
		vec3 T = normalize(vTangent);
		vec3 B = normalize(vBiTangent);
		
		//tangent basis matrix, rotation matrix to the plane of the tangent
		mat3 TBN = mat3(T, B, N);
		
		vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
	
		N = TBN * (texNormal * 2 - 1);
		
		normalC = vec4(N * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1);
	}
}