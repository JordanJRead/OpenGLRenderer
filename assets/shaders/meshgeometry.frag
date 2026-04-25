#version 430 core

in VertOut {
	mat3 normalMapMatrix;
	vec2 texCoords;
	vec3 worldPos;
	flat int sceneIndex;
} fragIn;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 diffuseColour;
uniform vec3 specularColour;
uniform float specularExponent;
uniform bool highlight;

out vec4 OutWorldPosSceneIndex;
out vec4 OutNormal;
out vec4 OutDiffuseColour;
out vec4 OutSpecularData;

void main() {
	vec4 diffuseSample = texture(diffuseTexture, fragIn.texCoords).rgba;
	vec4 specularSample = texture(specularTexture, fragIn.texCoords).rgba;
	if (diffuseSample.a < 0.5) {
		discard;
	}

	OutDiffuseColour = vec4(diffuseSample.rgb * diffuseColour + (highlight ? vec3(0.2) : vec3(0)), 1); //todo add to render settings
	OutSpecularData = vec4(specularSample.rgb * specularColour, specularExponent);

	vec3 normal = normalize(fragIn.normalMapMatrix * texture(normalTexture, fragIn.texCoords).xyz);
	OutWorldPosSceneIndex = vec4(fragIn.worldPos, fragIn.sceneIndex);
	OutNormal = vec4(normal, 1);
}