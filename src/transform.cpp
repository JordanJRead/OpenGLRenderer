#include "transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <array>
#include "imgui/imgui.h"

glm::mat4 Transform::getModelMatrix() const {
	glm::mat4 matrix{ 1 };
	matrix = glm::translate(matrix, mPosition);
	matrix = glm::eulerAngleXYZ(glm::radians(mRotationDeg.x), glm::radians(mRotationDeg.y), glm::radians(mRotationDeg.z)) * matrix;
	matrix = glm::scale(matrix, mScale);
	return matrix;
}

JSON Transform::toJSON() const {
	JSON json;
	json["position"]["x"] = mPosition.x;
	json["position"]["y"] = mPosition.y;
	json["position"]["z"] = mPosition.z;
	json["scale"]["x"] = mScale.x;
	json["scale"]["y"] = mScale.y;
	json["scale"]["z"] = mScale.z;
	json["rotation"]["x"] = mRotationDeg.x;
	json["rotation"]["y"] = mRotationDeg.y;
	json["rotation"]["z"] = mRotationDeg.z;
	return json;
}

Transform::Transform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotationDeg)
	: mPosition{ position }
	, mScale{ scale }
	, mRotationDeg{ rotationDeg }
{}

Transform::Transform(const JSON& json) {
	mPosition.x = json["position"].at("x");
	mPosition.y = json["position"].at("y");
	mPosition.z = json["position"].at("z");
	mScale.x = json["scale"].at("x");
	mScale.y = json["scale"].at("y");
	mScale.z = json["scale"].at("z");
	mRotationDeg.x = json["rotation"].at("x");
	mRotationDeg.y = json["rotation"].at("y");
	mRotationDeg.z = json["rotation"].at("z");
}

void Transform::renderUI() const {
	ImGui::Text("Transform");
	ImGui::DragFloat3("Position", (float*)&mPosition, 0.01f);
	ImGui::DragFloat3("Scale", (float*)&mScale, 0.01f);
	ImGui::DragFloat3("Rotation", (float*)&mRotationDeg, 1);
}