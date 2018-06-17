//brdf point light shader
#version 410 

//point light information
uniform vec3 lightPosition;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform float radius;

uniform vec3 cameraPosition;

float reflectionCoefficient = 0.5f;

//informational buffers from G-Pass
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D specularPowerTexture;

layout( location = 0 ) out vec3 lightOutput;
layout( location = 1 ) out vec3 specularOutput;

void main() 
{
	//get the tex coord from glFragcoord
	vec2 vTexCoord = gl_FragCoord.xy / textureSize(positionTexture, 0).xy; 

	vec3 normal = texture(normalTexture, vTexCoord).xyz;
	
	float mag = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
	
	if (mag < 0.1f)
	{
		lightOutput = vec3(0,0,0);
		specularOutput = vec3(0,0,0);
	}
	else
	{
		
		vec3 position = texture(positionTexture, vTexCoord).xyz;
		
		vec3 toLight = lightPosition - position;
		
		float falloff = 1 - min(1, length(toLight) / radius);
		
		vec4 specular = texture(specularTexture, vTexCoord);
		float specularPower = 1 / texture(specularPowerTexture, vTexCoord).r;

		//oren-nayar diffuse reflectance
		//------------------------------
		float roughness = sqrt(2 / (specularPower + 2));
		
		vec3 N = normalize(normal);
		vec3 E = normalize(cameraPosition - position);
		vec3 L = normalize(toLight);
		
		float NdL = max( 0.0f, dot( N, L ) );
		float NdE = max( 0.0f, dot( N, E ) );
		float R2 = roughness * roughness;
		
		//oren-nayar diffuse term
		float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
		float B = 0.45f * R2 / (R2 + 0.09f);
		
		//CX = max(0, cos(l,e))
		vec3 lightProjected = normalize( L - N * NdL );
		vec3 viewProjected = normalize( E - N * NdE);
		float CX = max( 0.0f, dot( lightProjected, viewProjected ) );
		
		//DX = sin(alpha) * tan(beta)
		float alpha = sin( max( acos( NdE ), acos( NdL ) ) );
		float beta = tan( min( acos( NdE ), acos( NdL ) ) );
		float DX = alpha * beta;
		
		//calculate oren-nayar, replaces the phong lambertian term
		float orenNayar = NdL * (A + B * CX * DX);
		//------------------------------
		
		float lambertTerm = max(0, min(1, dot(N, -L)));
		
		//relative vector from the fragment position to the camera
		vec3 V = normalize(cameraPosition - position);
		
		//direction of light reflecting off the surface
		vec3 R = reflect(-L, N);
		
		float specularTerm = pow(max(0, dot(R,V)), specularPower);
		
		//cook torrance specular reflectance
		//------------------------------
		vec3 H = normalize(E + L);
		
		float NdH = max( 0.0f, dot( N, H ) );
		float NdH2 = NdH * NdH;
		float e = 2.71828182845904523536028747135f;
		float pi = 3.1415926535897932384626433832f;
		
		//Beckman's Distribution Function D
		float exponent = -(1 - NdH2) / (NdH2 * R2);
		float D = pow( e, exponent ) / (R2 * NdH2 * NdH2);
		
		//Fresnel Term F
		float F = reflectionCoefficient + (1 - reflectionCoefficient) * pow(1 - NdE, specularPower);
		
		//Geometric Attenuation Factor G
		float X = 2.0f * NdH / dot( E, H );
		float G = min(1, min(X * NdL, X * NdE));
		
		//Calculate Cook-Torrance
		float cookTorrance = max( (D*G*F) / (NdE * pi), 0.0f );
		//------------------------------
	
		lightOutput = lightDiffuse.rgb * orenNayar * falloff;
		specularOutput = lightSpecular.rgb * specular.rgb * cookTorrance * falloff;	
	}
}

