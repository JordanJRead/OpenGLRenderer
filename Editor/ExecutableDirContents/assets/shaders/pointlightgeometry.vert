#version 430 core

layout(location = 0) in vec3 vPos;

#include "buffers.glsl"

uniform vec3 position;

out vec3 worldPos;

void main() {
	worldPos = vPos * (renderSettings.pointLightScale) + position;
	gl_Position = cameraData.projection * cameraData.view * vec4(worldPos, 1); 
}