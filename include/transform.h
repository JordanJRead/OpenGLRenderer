#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"

class Transform {
public:
	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mRotationDeg;

	glm::mat4 getModelMatrix() const;
};

#endif