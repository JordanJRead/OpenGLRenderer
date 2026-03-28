#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;

out VertOut {
	vec3 normal;
} vertOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(vPos + vNormal * 0.0001, 1);
	vertOut.normal = normalize(mat3(transpose(inverse(model))) * vNormal);
}