#version 430 core

in VertOut {
	vec2 texCoords;
} fragIn;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform vec3 diffuseColour;

void main() {
	FragColour = vec4(texture(diffuseTexture, fragIn.texCoords).rgb * diffuseColour, 1);
}