#version 430 core

in vec2 texCoord;
out vec4 FragColour;

uniform sampler2D worldPosBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D albedoBuffer;

struct DirectionalLight {
	vec3 dirTo;
	vec3 colour;
};
uniform DirectionalLight directionalLight;

layout(binding = 0, std430) buffer PointLights {
	float data[]; // x, y, z, r, g, b
} pointLights;

vec3 getLightFromPointLight(vec3 objectPos, vec3 objectColor, vec3 objectNormal, vec3 lightPos, vec3 lightColour);
vec3 getLightFromDirectionalLight(vec3 objectPos, vec3 objectColor, vec3 objectNormal, vec3 dirToLight, vec3 lightColour);

void main() {
	vec3 albedo = texture(albedoBuffer, texCoord).rgb;
	vec3 normal = texture(normalBuffer, texCoord).rgb;
	vec3 worldPos = texture(worldPosBuffer, texCoord).rgb;

	if (normal == vec3(0, 0, 0)) {
		FragColour = vec4(albedo, 1);
		return;
	}

	vec3 resultColour = getLightFromDirectionalLight(worldPos, albedo, normal, directionalLight.dirTo, directionalLight.colour);

	for (int i = 0; i < pointLights.data.length() / 6 ; ++i) {
		vec3 lightPos    = vec3(pointLights.data[i * 6 + 0], pointLights.data[i * 6 + 1], pointLights.data[i * 6 + 2]);
		vec3 lightColour = vec3(pointLights.data[i * 6 + 3], pointLights.data[i * 6 + 4], pointLights.data[i * 6 + 5]);
		resultColour += getLightFromPointLight(worldPos, albedo, normal, lightPos, lightColour);
	}

	vec3 gammaCorrectedColour = pow(resultColour, vec3(1/2.2));
	FragColour = vec4(gammaCorrectedColour, 1);
}

// TODO specular
vec3 getLightFromPointLight(vec3 objectPos, vec3 objectColor, vec3 objectNormal, vec3 lightPos, vec3 lightColour) {
	return objectColor * max(0, dot(objectNormal, normalize(lightPos - objectPos))) * lightColour / pow(length(objectPos - lightPos), 2);
}
vec3 getLightFromDirectionalLight(vec3 objectPos, vec3 objectColor, vec3 objectNormal, vec3 dirToLight, vec3 lightColour) {
	return objectColor * max(0, dot(objectNormal, dirToLight)) * lightColour;
}