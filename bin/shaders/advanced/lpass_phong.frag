///hybrid deferred phong shader
#version 410 

//0 = DIRECTIONAL, 1 = POINT, 2 = SPOT
uniform int lightType = 0;

//--------------------------LIGHT INFO--------------------------
//position for point and spot
uniform vec3 lightPosition;

//direction for directional and spot
uniform vec3 lightDirection;

//all required
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

//required for point and spot
uniform float range;

//required for spot
uniform float minCone;
uniform float maxCone;
//--------------------------------------------------------------

uniform vec3 cameraPosition;

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

	//sample normal from buffer
	vec3 normal = texture(normalTexture, vTexCoord).xyz;
	
	//check normal isn't < 0.1f (no geometry was rendered here)
	float mag = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
	
	if (mag < 0.1f)
	{
		lightOutput = vec3(0,0,0);
		specularOutput = vec3(0,0,0);
	}
	else
	{
		vec3 N = normalize(normal);
	    
		vec3 position = texture(positionTexture, vTexCoord).xyz;
		
		vec3 toLight = lightPosition - position;
		
		vec3 L = normalize(-lightDirection);
		
		float falloff = 1.0f;
		
		//point/spot light distance filtering
		if (lightType >= 1)
		{	
			falloff = 1 - min(1, length(toLight) / range);
			L = normalize(toLight);
		}
		
		float angleFalloff = 1.0f;
		
		//spot light angle filtering
		if (lightType == 2)
		{
			//get the maximum/minumum inverse dot products
			float angleDot = dot(L, lightDirection);
			float minDot = sin(minCone * 0.5f);
			float maxDot = sin(maxCone * 0.5f);
			
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
		
		vec4 specular = texture(specularTexture, vTexCoord);
		float specularPower = 1 / texture(specularPowerTexture, vTexCoord).r;
		
		float specularTerm = pow(max(0, dot(R,V)), specularPower);
		
		lightOutput = lightDiffuse.rgb * lambertTerm * falloff * angleFalloff;
		specularOutput = lightSpecular.rgb * specular.rgb * specularTerm * falloff * angleFalloff;	
	}
}

