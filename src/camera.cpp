#include "camera.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"

Camera::Camera(const glm::vec3& position, double horizontalFOVDeg, double lookSensitivity)
	: mPosition{ position }
	, mHorizontalFOVDeg{ horizontalFOVDeg }
	, mLookSensitivity{ lookSensitivity }
{}

void Camera::update(GLFWwindow* window, float deltaTime) {
	double currentMouseX, currentMouseY;
	glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
	
	if (!mIsFirstUpdate) {
		double dX = currentMouseX - mPrevMouseX;
		double dY = currentMouseY - mPrevMouseY;
		mYawDeg += dX * mLookSensitivity;
		mPitchDeg -= dY * mLookSensitivity;

		if (mPitchDeg > 85) {
			mPitchDeg = 85;
		}
		if (mPitchDeg < -85) {
			mPitchDeg = -85;
		}
	}
	else {
		mIsFirstUpdate = false;
	}
	mPrevMouseX = currentMouseX;
	mPrevMouseY = currentMouseY;

	glm::vec3 flatForward{ getForward() };
	flatForward.y = 0;
	flatForward = glm::normalize(flatForward);
	glm::vec3 flatRight{ -flatForward.z, 0, flatForward.x };

	glm::vec3 moveDirection{ 0, 0, 0 };
	if (glfwGetKey(window, GLFW_KEY_W)) {
		moveDirection += flatForward;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		moveDirection -= flatForward;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		moveDirection += flatRight;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		moveDirection -= flatRight;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		moveDirection += glm::vec3{ 0, 1, 0 };
	}
	if (glfwGetKey(window, GLFW_KEY_C)) {
		moveDirection -= glm::vec3{ 0, 1, 0 };
	}

	if (glm::length(moveDirection) != 0) {
		mPosition += glm::normalize(moveDirection) * deltaTime;
	}
}

glm::vec3 Camera::getForward() const {
	glm::vec3 forward;
	forward.x =  glm::sin(glm::radians(mYawDeg)) * glm::cos(glm::radians(mPitchDeg));
	forward.y =  glm::sin(glm::radians(mPitchDeg));
	forward.z = -glm::cos(glm::radians(mYawDeg)) * glm::cos(glm::radians(mPitchDeg));
	return forward;
}

glm::mat4 Camera::getViewMatrix() const {
	glm::vec3 forward{ getForward() };
	return glm::lookAt(mPosition, mPosition + forward, glm::vec3{ 0, 1, 0 });
}

glm::mat4 Camera::getProjectionMatrix(int screenWidth, int screenHeight) const {
	return glm::perspective(glm::radians((float)mHorizontalFOVDeg), (float)screenWidth / screenHeight, 0.1f, 1000.0f);
}