#version 430 core

in VertOut {
	vec3 normal;
	vec2 texCoords;
} fragIn;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform vec3 diffuseColour;

void main() {
	vec3 colour = texture(diffuseTexture, fragIn.texCoords).rgb;
	colour *= (dot(fragIn.normal, vec3(0, 1, 0)) + 1) / 2;
	FragColour = vec4(colour, 1);

	// testing
	FragColour = vec4(texture(normalTexture, fragIn.texCoords).rgb, 1);
}