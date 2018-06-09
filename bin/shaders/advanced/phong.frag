//phong shader
#version 410 
 
in vec2 vTexCoord;

//directional light information
uniform vec3 lightDirection;
uniform vec4 lightDiffuse;

//informational buffers from G-Pass
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

layout( location = 0) out vec3 lightOutput;

void main() 
{

	vec3 normal = texture(normalTexture, vTexCoord).xyz;
	
	float mag = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
	
	if (mag < 0.1f)
	{
		lightOutput = vec3(0,0,0);
	}
	else
	{
		normal = normalize(normal);
	
		vec3 L = normalize(lightDirection);
		
		vec3 position = texture(positionTexture, vTexCoord).xyz;
		
		vec3 pt = texture(positionTexture, vTexCoord).rgb;
		
		float d = max(0, dot(normal, -L));
		lightOutput = lightDiffuse.rgb * d;
	}
}

