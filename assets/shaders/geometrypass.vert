#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec2 vTexCoord;

out VertOut {
	vec3 normal;
	vec2 texCoords;
	vec3 worldPos;
} vertOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vertOut.worldPos = (model * vec4(vPos + vNormal * 0.0001, 1)).xyz;
	gl_Position = projection * view * vec4(vertOut.worldPos, 1);
	vertOut.texCoords = vTexCoord;
	vertOut.normal = normalize(mat3(transpose(inverse(model))) * vNormal);
}