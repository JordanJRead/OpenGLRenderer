#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec3 vBitangent;
layout(location = 4) in vec2 vTexCoord;

#include "buffers.glsl"

uniform mat4 model;

out VertOut {
	mat3 normalMapMatrix;
	vec2 texCoords;
	vec3 worldPos;
} vertOut;

void main() {
	vertOut.worldPos = (model * vec4(vPos + vNormal * 0.0001, 1)).xyz;
	gl_Position = cameraData.projection * cameraData.view * vec4(vertOut.worldPos, 1);
	vertOut.texCoords = vTexCoord;

	mat3 normalMatrix = mat3(transpose(inverse(model)));

	vertOut.normalMapMatrix = mat3(
		normalize(normalMatrix * vTangent),
		normalize(normalMatrix * vBitangent),
		normalize(normalMatrix * vNormal)
//		vTangent,
//		vBitangent,
//		vNormal
	);
}