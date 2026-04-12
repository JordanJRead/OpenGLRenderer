#version 430 core

in VertOut {
	mat3 normalMapMatrix;
	vec2 texCoords;
	vec3 worldPos;
} fragIn;

out vec4 OutWorldPos;
out vec4 OutNormal;
out vec4 OutDiffuseColour;
out vec4 OutSpecularData;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 diffuseColour;
uniform vec3 specularColour;
uniform float specularExponent;

uniform bool doNormalMapping;

void main() {
	vec4 diffuseSample = texture(diffuseTexture, fragIn.texCoords).rgba;
	vec4 specularSample = texture(specularTexture, fragIn.texCoords).rgba;
	if (diffuseSample.a < 0.5) {
		discard;
	}

	OutDiffuseColour = vec4(diffuseSample.rgb * diffuseColour, 1);
	OutSpecularData = vec4(specularSample.rgb * specularColour, specularExponent);

	vec3 normal = doNormalMapping ? normalize(fragIn.normalMapMatrix * texture(normalTexture, fragIn.texCoords).xyz) : normalize(fragIn.normalMapMatrix[2]);
	OutWorldPos = vec4(fragIn.worldPos, 1);
	OutNormal = vec4(normal, 1);
}