#version 430 core

#include "buffers.glsl"

in vec3 worldPos;

uniform float objectPtrFirst;
uniform float objectPtrSecond;

uniform vec3 colour;
uniform bool highlight;

layout(location=0) out vec4 OutWorldPos;
layout(location=1) out vec4 OutNormal;
layout(location=2) out vec4 OutDiffuseColour;
layout(location=3) out vec4 OutSpecularData;
layout(location=4) out vec2 OutObjectPtr;

void main() {
	OutWorldPos = vec4(worldPos, 1);
	OutNormal = vec4(0, 0, 0, 1);
	OutDiffuseColour = vec4(colour - (highlight ? renderSettings.highlightColour : vec3(0)), 1);

	OutObjectPtr = vec2(objectPtrFirst, objectPtrSecond);
}