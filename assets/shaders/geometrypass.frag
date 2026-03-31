#version 430 core

in VertOut {
	vec3 normal;
	vec2 texCoords;
	vec3 worldPos;
} fragIn;

out vec4 OutWorldPos;
out vec4 OutNormal;
out vec4 OutAlbedo;

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
	OutAlbedo = vec4(colour3, 1);
	OutWorldPos = vec4(fragIn.worldPos, 1);
	OutNormal = vec4(normalize(fragIn.normal), 1);
}