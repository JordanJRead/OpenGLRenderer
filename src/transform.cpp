#include "transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

glm::mat4 Transform::getModelMatrix() {
	glm::mat4 matrix{ 1 };
	matrix = glm::scale(matrix, mScale);
	matrix = glm::eulerAngleXYZ(glm::radians(mRotationDeg.x), glm::radians(mRotationDeg.y), glm::radians(mRotationDeg.z)) * matrix;
	matrix = glm::scale(matrix, mScale);
	return matrix;
}