#ifndef BUFFERS_GLSL_INCLUDE
#define BUFFERS_GLSL_INCLUDE

layout(binding = 0, std430) buffer PointLights {
	float data[]; // x, y, z, r, g, b
} pointLights;

layout(binding = 1, std140) uniform CameraData {
	uniform mat4 view;
	uniform mat4 projection;
	uniform vec3 position;
} cameraData;

#endif