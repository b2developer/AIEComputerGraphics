//deferred shader
#version 410 
 
in vec4 position;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

uniform int useTexture = 0;
uniform int useAmbientTexture = 0;
uniform int useNormalTexture = 0;
uniform int useSpecularTexture = 0;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower = 0.0f;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

layout(location = 0) out vec3 ambientC;
layout(location = 1) out vec3 albedoC;
layout(location = 2) out vec3 positionC;
layout(location = 3) out vec3 normalC;
layout(location = 4) out vec3 specularC;
layout(location = 5) out float specularPowerC;

void main() 
{
	int inaT = 1 - useAmbientTexture;

	ambientC = (texture(ambientTexture, vTexCoord).rgb + vec3(inaT, inaT, inaT)) * Ka;

	int inT = 1 - useTexture;

	albedoC = (texture(diffuseTexture, vTexCoord).rgb + vec3(inT, inT, inT)) * Kd;

	vec3 sposition = position.xyz;
	
	positionC = sposition;

	vec3 N = normalize(vNormal);

	if (useNormalTexture == 0)
	{
		normalC = N;
	}
	else
	{
		vec3 T = normalize(vTangent);
		vec3 B = normalize(vBiTangent);
		
		//tangent basis matrix, rotation matrix to the plane of the tangent
		mat3 TBN = mat3(T, B, N);
		 
		vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
	
		N = TBN * (texNormal * 2 - 1);
		
		normalC = N;
	}
	
	int insT = 1 - useSpecularTexture;

	specularC = (texture(specularTexture, vTexCoord).rgb + vec3(insT, insT, insT)) * Ks;
	specularPowerC = 1 / (specularPower + 0.00001f);
}