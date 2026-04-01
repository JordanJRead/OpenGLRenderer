#version 430 core

uniform vec3 colour;

out vec4 OutWorldPos;
out vec4 OutNormal;
out vec4 OutAlbedo;

in vec3 worldPos;

void main() {
	OutAlbedo = vec4(colour, 1);
	OutWorldPos = vec4(worldPos, 1);
	OutNormal = vec4(0, 0, 0, 1);
}