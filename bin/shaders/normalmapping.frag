 //normal mapping fragment shader
 #version 410 
 
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

out vec4 FragColour;

uniform int useTexture = 0;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; //ambient colour
uniform vec3 Kd; //diffuse colour, what colour is the texture when light shines on it
uniform vec3 Ks; //specular colour, what colour does the texture shine with direct light reflection
uniform float specularPower;

//shading from a simulated light
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 lightDirection;

uniform vec3 cameraPosition;
 
void main() 
{  
	//normal of the face
	vec3 N = normalize(vNormal);
	
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	
	//direction of the light
	vec3 L = normalize(lightDirection);
	
	//tangent basis matrix, rotation matrix to the plane of the tangent
	mat3 TBN = mat3(T, B, N);
	
	int iut = 1 - useTexture;
	
	//get the diffuse colour
	vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb + vec3(iut, iut, iut);
	vec3 texSpecular = texture(specularTexture, vTexCoord).rgb + vec3(iut, iut, iut);
	vec3 texNormal = texture(normalTexture, vTexCoord).rgb + vec3(iut, iut, iut);
	
	if (useTexture == 1)
	{
		N = TBN * (texNormal * 2 - 1);
	}
	
	//calculate lambert term
	float lambertTerm = max(0, min(1, dot(N, -L)));
	
	//relative vector from fragment position to camera
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	
	//direction of light after reflecting off surface
	vec3 R = reflect(L, N);
	
	//shininess of the material
	float specularTerm = pow(max(0, dot(R, V)), specularPower);
	
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
	vec3 specular = Is * Ks * texSpecular * specularTerm;

	FragColour = vec4(ambient + diffuse + specular, 1);
}