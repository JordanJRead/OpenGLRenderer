#version 430 core

layout(location = 0) in vec3 vPos;

uniform vec3 position;
uniform vec3 scale;

uniform mat4 view;
uniform mat4 projection;

out vec3 worldPos;

void main() {
	worldPos = vPos * scale + position;
	gl_Position = projection * view * vec4(worldPos, 1); 
}