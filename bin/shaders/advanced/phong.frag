//phong shader
#version 410 
 
in vec2 vTexCoord;

//directional light information
uniform vec3 lightDirection;
uniform vec4 lightDiffuse;

//informational buffers from G-Pass
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

out vec4 lightOutput;

void main() 
{

	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	
	normal = normal - vec3(0.5f, 0.5f, 0.5f);
	normal *= 2.0f;
	
	vec3 L = normalize(lightDirection);
	
	vec3 position = texture(positionTexture, vTexCoord).xyz;
	
	vec3 pt = texture(positionTexture, vTexCoord).rgb;
	
	float d = max(0, dot(normal, -L));
	lightOutput = vec4(lightDiffuse.rgb * d, 1) + vec4(pt,1) * 0.001f;
}