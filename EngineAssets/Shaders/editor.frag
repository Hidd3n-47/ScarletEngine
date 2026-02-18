#version 330 core

const int TOON_COLOR_LEVELS = 4;
const float TOON_SCALE_FACTOR = 1.0f / TOON_COLOR_LEVELS;

in vec2 textureUv;
in vec3 fragNormal;
in vec3 vertexPosition;

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

uniform vec3 uCameraPosition;
uniform BaseLight uLight;
uniform Material  uMaterial;
uniform sampler2D uTexture;
uniform float uRimLightPower;

float CalculateRimFactor(vec3 pixelToCamera, vec3 normal)
{
	float rimFactor = dot(pixelToCamera, normal);
	rimFactor = 1.0f - rimFactor;
	rimFactor = max(0.0f, rimFactor);
	rimFactor = pow(rimFactor, uRimLightPower);
	return rimFactor;
}

void main()
{
	vec4 ambientColor = vec4(uMaterial.ambientColor, 1.0f)
					  * vec4(uLight.color, 1.0f)
					  * uLight.ambientIntensity;

	float diffuseFactor = dot(fragNormal, -uLight.direction);
	diffuseFactor = ceil(diffuseFactor * TOON_COLOR_LEVELS) * TOON_SCALE_FACTOR;
	
	vec4 diffuseColor  = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	vec3 pixelToCamera = normalize(uCameraPosition - vertexPosition);
	vec3 lightReflect  = normalize(reflect(uLight.direction, fragNormal));
	
	if(diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(uMaterial.diffuseColor, 1.0f)
					 * vec4(uLight.color, 1.0f)
					 * uLight.diffuseIntensity
					 * diffuseFactor;
	}
	
	vec4  rimColor  = diffuseColor * CalculateRimFactor(pixelToCamera, fragNormal);
	
    color = texture(uTexture, textureUv)
		  * clamp(ambientColor + diffuseColor + rimColor, 0.0f, 1.0f);
	
	//color = vec4((fragNormal * 0.5f + 0.5f) * diffuseFactor, 1.0f);
}