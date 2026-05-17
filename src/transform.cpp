#include "transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <array>
#include "imgui/imgui.h"
#include "jsonhelpers.h"

glm::mat4 Transform::getModelMatrix() const {
	glm::mat4 matrix{ 1 };
	matrix = glm::translate(matrix, mPosition);
	matrix = glm::eulerAngleXYZ(glm::radians(mRotationDeg.x), glm::radians(mRotationDeg.y), glm::radians(mRotationDeg.z)) * matrix;
	matrix = glm::scale(matrix, mScale);
	return matrix;
}

JSON Transform::toJSON() const {
	JSON json;
	json["position"] = JSONHelpers::fromVec3(mPosition);
	json["scale"] = JSONHelpers::fromVec3(mScale);
	json["rotation"] = JSONHelpers::fromVec3(mRotationDeg);
	return json;
}

Transform::Transform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotationDeg)
	: mPosition{ position }
	, mScale{ scale }
	, mRotationDeg{ rotationDeg }
{}

Transform::Transform(const JSON& json) {
	mPosition = JSONHelpers::toVec3(json.at("position"));
	mScale = JSONHelpers::toVec3(json.at("scale"));
	mRotationDeg = JSONHelpers::toVec3(json.at("rotation"));
}

void Transform::renderUI() const {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.01f);
		ImGui::DragFloat3("Scale", (float*)&mScale, 0.01f);
		ImGui::DragFloat3("Rotation", (float*)&mRotationDeg, 1);
	}
}