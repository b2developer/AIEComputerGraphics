//forward phong alpha shader
#version 410 

//maximum lights that will be considered for transparent lighting
const int MAX_LIGHTS = 24;

struct Light
{
	//0 = DIRECTIONAL, 1 = POINT, 2 = SPOT
	int lightType;
	
	//--------------------------LIGHT INFO--------------------------
	//position for point and spot
	vec3 lightPosition;
	
	//direction for directional and spot
	vec3 lightDirection;
	
	//all required
	vec3 lightDiffuse;
	vec3 lightSpecular;
	
	//required for point and spot
	float range;
	
	//required for spot
	float minCone;
	float maxCone;
	//--------------------------------------------------------------
};
 
in vec4 position;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

uniform Light lights[MAX_LIGHTS];
uniform vec3 cameraPosition;

uniform int useTexture = 0;
uniform int useAmbientTexture = 0;
uniform int useNormalTexture = 0;
uniform int useSpecularTexture = 0;
uniform int useAlphaTexture;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float opacity;
uniform float specularPower = 0.0f;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;

layout(location = 6) out vec4 transparentC;
layout(location = 7) out vec4 blendC;

void main() 
{

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		int inaT = 1 - useAmbientTexture;

		vec3 ambient = (texture(ambientTexture, vTexCoord).rgb + vec3(inaT, inaT, inaT)) * Ka;
	
		int inT = 1 - useTexture;
	
		vec3 albedo = (texture(diffuseTexture, vTexCoord).rgb + vec3(inT, inT, inT)) * Kd;
	
		vec3 position = position.xyz;
	
		vec3 N = normalize(vNormal);
	
		if (useNormalTexture == 1)
		{
			vec3 T = normalize(vTangent);
			vec3 B = normalize(vBiTangent);
			
			//tangent basis matrix, rotation matrix to the plane of the tangent
			mat3 TBN = mat3(T, B, N);
			
			vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
		
			N = TBN * (texNormal * 2 - 1);
		}
		
		int insT = 1 - useSpecularTexture;
	
		vec3 specular = (texture(specularTexture, vTexCoord).rgb + vec3(insT, insT, insT)) * Ks;
		
		//----------ADD LIGHT------------
		vec3 toLight = lights[i].lightPosition - position;
		
		vec3 L = normalize(-lights[i].lightDirection);
		
		float falloff = 1.0f;
		
		//point/spot light distance filtering
		if (lights[i].lightType >= 1)
		{	
			falloff = 1 - min(1, length(toLight) / lights[i].range);
			L = normalize(toLight);
		}
		
		float angleFalloff = 1.0f;
		
		//spot light angle filtering
		if (lights[i].lightType == 2)
		{
			//get the maximum/minumum inverse dot products
			float angleDot = dot(L, lights[i].lightDirection);
			float minDot = sin(lights[i].minCone * 0.5f);
			float maxDot = sin(lights[i].maxCone * 0.5f);
			
			//calculate angle falloff
			float d = 1 - angleDot;
			float s = max(d, minDot) - minDot;
			float sc = s / maxDot;
			angleFalloff = max(1 - sc, 0);
		}
	
		float lambertTerm = max(0, min(1, dot(N, L)));
		
		//relative vector from the fragment position to the camera
		vec3 V = normalize(cameraPosition - position);
		
		//direction of light reflecting off the surface
		vec3 R = reflect(-L, N);
		
		float specularTerm = pow(max(0, dot(R,V)), specularPower);
		
		vec3 light = lights[i].lightDiffuse.rgb * lambertTerm * falloff * angleFalloff;
		vec3 specLight = lights[i].lightSpecular.rgb * specular.rgb * specularTerm * falloff * angleFalloff;
		//-------------------------------
		
		transparentC += vec4(ambient + albedo * light + specLight, 0.0f);
	}

	int inalpT = 1 - useAlphaTexture;

	vec3 alpha = (texture(alphaTexture, vTexCoord).rgb + vec3(inalpT, inalpT, inalpT)) * opacity;

	transparentC.a = alpha.r;
	
	blendC = vec4(1,0,0,transparentC.a);
}