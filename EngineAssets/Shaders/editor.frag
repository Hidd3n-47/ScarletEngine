#version 330 core

in vec2 textureUv;
in vec3 fragNormal;

out vec4 color;

struct BaseLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
};

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
};

uniform BaseLight uLight;
uniform Material  uMaterial;
uniform sampler2D uTexture;

void main()
{
	vec4 ambientColor = vec4(uMaterial.ambientColor, 1.0f)
					  * vec4(uLight.color, 1.0f)
					  * uLight.ambientIntensity;

	float diffuseFactor = dot(fragNormal, -uLight.direction);
	
	vec4 diffuseColor = diffuseFactor <= 0 ? vec4(0.0f, 0.0f, 0.0f, 0.0f)
										   : vec4(uMaterial.diffuseColor, 1.0f)
					                       * vec4(uLight.color, 1.0f)
					                       * uLight.diffuseIntensity;
	
    color = texture(uTexture, textureUv)
		  * (ambientColor + diffuseColor);
	
	//color = vec4((fragNormal * 0.5f + 0.5f) *diffuseFactor, 1.0f);
}