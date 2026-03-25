#version 430 core

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

out VertOut {
	vec3 normal;
	vec2 texCoords;
} vertOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(vPos + vNormal * 0.0001, 1);
	vertOut.texCoords = vTexCoord;
	vertOut.normal = normalize(mat3(transpose(inverse(model))) * vNormal);
}