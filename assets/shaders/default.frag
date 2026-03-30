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
	vec4 colour = texture(diffuseTexture, fragIn.texCoords).rgba;
	if (colour.a < 0.5) {
		discard;
	}
	vec3 colour3 = colour.rgb;// * diffuseColour;
	colour3 *= (dot(fragIn.normal, vec3(0, 1, 0)) + 1) / 2; // fake lighting for now
	FragColour = vec4(colour3, 1);
}