#version 430 core

layout(location = 0) in vec3 vPos;

#include "buffers.glsl"

uniform vec3 position;
uniform vec3 scale;
uniform int sceneIndex;

out vec3 worldPos;
out flat int outSceneIndex;

void main() {
	outSceneIndex = sceneIndex;
	worldPos = vPos * scale + position;
	gl_Position = cameraData.projection * cameraData.view * vec4(worldPos, 1); 
}