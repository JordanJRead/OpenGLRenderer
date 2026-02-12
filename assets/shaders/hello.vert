#version 460

in layout(location = 0) vec2 vertPos;
in layout(location = 1) vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {
	gl_Position = vec4(vertPos, 0, 1);
	fragTexCoord = vertTexCoord;
}