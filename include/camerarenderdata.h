#ifndef CAMERA_RENDER_DATA_H
#define CAMERA_RENDER_DATA_H

#include "glm/glm.hpp"

struct CameraRenderData {
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 position;

	bool operator==(const CameraRenderData&) const = default;
};

#endif