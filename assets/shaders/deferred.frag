#version 430 core

in vec2 texCoord;
out vec4 FragColour;

uniform sampler2D worldPosBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D diffuseBuffer;
uniform sampler2D specularDataBuffer;

struct DirectionalLight {
	vec3 dirTo;
	vec3 colour;
};
uniform DirectionalLight directionalLight;

uniform vec3 ambientLightColour;
uniform vec3 cameraPos;

layout(binding = 0, std430) buffer PointLights {
	float data[]; // x, y, z, r, g, b
} pointLights;

vec3 getLightFromPointLight(vec3 objectPos, vec3 diffuseColour, vec3 specularColour, float specularExponent, vec3 objectNormal, vec3 lightPos, vec3 lightColour);
vec3 getLightFromDirectionalLight(vec3 objectPos, vec3 diffuseColour, vec3 specularColour, float specularExponent, vec3 objectNormal, vec3 dirToLight, vec3 lightColour);

void main() {
	vec3 diffuseAlbedo = texture(diffuseBuffer, texCoord).rgb;
	vec4 specularData = texture(specularDataBuffer, texCoord);
	vec3 specularColour = specularData.rgb;
	float specularExponent = specularData.w;
	vec3 normal = texture(normalBuffer, texCoord).rgb;
	vec3 worldPos = texture(worldPosBuffer, texCoord).rgb;

	if (normal == vec3(0, 0, 0)) {
		FragColour = vec4(diffuseAlbedo, 1);
		return;
	}

	vec3 resultColour = diffuseAlbedo * ambientLightColour;
	resultColour += getLightFromDirectionalLight(worldPos, diffuseAlbedo, specularColour, specularExponent, normal, directionalLight.dirTo, directionalLight.colour);

	for (int i = 0; i < pointLights.data.length() / 6 ; ++i) {
		vec3 lightPos    = vec3(pointLights.data[i * 6 + 0], pointLights.data[i * 6 + 1], pointLights.data[i * 6 + 2]);
		vec3 lightColour = vec3(pointLights.data[i * 6 + 3], pointLights.data[i * 6 + 4], pointLights.data[i * 6 + 5]);
		resultColour += getLightFromPointLight(worldPos, diffuseAlbedo, specularColour, specularExponent, normal, lightPos, lightColour);
	}

	vec3 gammaCorrectedColour = pow(resultColour, vec3(1/2.2));
	FragColour = vec4(gammaCorrectedColour, 1);
}

vec3 getLightFromPointLight(vec3 objectPos, vec3 diffuseColour, vec3 specularColour, float specularExponent, vec3 objectNormal, vec3 lightPos, vec3 lightColour) {
	vec3 attenuatedLightColour = lightColour / pow(length(objectPos - lightPos), 2);
	vec3 dirToLight = normalize(lightPos - objectPos);
	vec3 dirToView = normalize(cameraPos - objectPos);
	
	vec3 diffuseLight = diffuseColour * max(0, dot(objectNormal, dirToLight)) * attenuatedLightColour;

	vec3 reflectedDir = reflect(-dirToLight, objectNormal);
	vec3 specularLight = specularColour * pow(max(0, dot(reflectedDir, dirToView)), specularExponent) * attenuatedLightColour;
	
	return diffuseLight + specularLight;
}
vec3 getLightFromDirectionalLight(vec3 objectPos, vec3 diffuseColour, vec3 specularColour, float specularExponent, vec3 objectNormal, vec3 dirToLight, vec3 lightColour) {
	vec3 dirToView = normalize(cameraPos - objectPos);
	
	vec3 diffuseLight = diffuseColour * max(0, dot(objectNormal, dirToLight)) * lightColour;

	vec3 reflectedDir = reflect(-dirToLight, objectNormal);
	vec3 specularLight = specularColour * pow(max(0, dot(reflectedDir, dirToView)), specularExponent) * lightColour;
	
	return diffuseLight + specularLight;
}