#version 430 core

in vec2 texCoord;

uniform sampler2D source;

out vec4 FragColour;

void main() {
	vec3 sourceColour = texture(source, texCoord).rgb;
	vec3 newColour = pow(sourceColour, vec3(1/2.2));
	FragColour = vec4(newColour, 1);
}