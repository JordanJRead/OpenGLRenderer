#version 430 core

in vec3 worldPos;
in flat int outSceneIndex;

uniform vec3 colour;

out vec4 OutWorldPosSceneIndex;
out vec4 OutNormal;
out vec4 OutDiffuseColour;
out vec4 OutSpecularData;

void main() {
	OutDiffuseColour = vec4(colour, 1);
	OutWorldPosSceneIndex = vec4(worldPos, outSceneIndex);
	OutNormal = vec4(0, 0, 0, 1);
}