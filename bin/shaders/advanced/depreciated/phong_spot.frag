//phong shader
#version 410 

//spot light information
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform float range;
uniform float minCone;
uniform float maxCone;

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

	vec3 normal = texture(normalTexture, vTexCoord).xyz;
	
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
		
		float falloff = 1 - min(1, length(toLight) / range);
		
		vec3 L = normalize(toLight);
		
		float angleDot = dot(L, lightDirection);
		float minDot = sin(minCone * 0.5f);
		float maxDot = sin(maxCone * 0.5f);
		
		//calculate angle falloff
		float d = 1 - angleDot;
		float s = max(d, minDot) - minDot;
		float sc = s / maxDot;
		float angleFalloff = max(1 - sc, 0);
	
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

