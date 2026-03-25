#version 430 core

in VertOut {
	vec3 normal;
} fragIn;

out vec4 FragColour;

uniform vec3 diffuseColour;

void main() {
	vec3 colour = diffuseColour;
	colour *= (dot(fragIn.normal, vec3(0, 1, 0)) + 1) / 2;
	FragColour = vec4(colour, 1);
}