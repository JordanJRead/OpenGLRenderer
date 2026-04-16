#version 430 core

in vec3 worldPos;

uniform vec3 colour;

out vec4 OutWorldPos;
out vec4 OutNormal;
out vec4 OutDiffuseColour;
out vec4 OutSpecularColour;
out vec4 OutAmbientColour;

void main() {
	OutAmbientColour = vec4(colour, 1);
	OutWorldPos = vec4(worldPos, 1);
	OutNormal = vec4(0, 0, 0, 1);
}