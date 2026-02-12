#version 460

in vec2 fragTexCoord;
out vec4 FragColour;

uniform sampler2D image;

void main() {
	FragColour = vec4(texture(image, fragTexCoord).rgb, 1);
}