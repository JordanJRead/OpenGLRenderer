#include "camera.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"
#include "inputs.h"

Camera::Camera(const glm::vec3& position, double horizontalFOVDeg, double lookSensitivity, int screenWidth, int screenHeight)
	: mPosition{ position }
	, mHorizontalFOVDeg{ horizontalFOVDeg }
	, mSensitivity{ lookSensitivity }
	, mScreenWidth{ screenWidth }
	, mScreenHeight{ screenHeight }
{}

void Camera::loadJSONData(const JSON& json) {
	mHorizontalFOVDeg = json.at("horizontalFOVDeg");
	mSensitivity = json.at("sensitivity");
	mPosition.x = json["position"].at("x");
	mPosition.y = json["position"].at("y");
	mPosition.z = json["position"].at("z");
}

JSON Camera::toJSON() const {
	JSON json;
	json["position"]["x"] = mPosition.x;
	json["position"]["y"] = mPosition.y;
	json["position"]["z"] = mPosition.z;
	json["sensitivity"] = mSensitivity;
	json["horizontalFOVDeg"] = mHorizontalFOVDeg;
	return json;
}

void Camera::update(GLFWwindow* window, const Inputs& inputs, float deltaTime) {

	if (inputs.checkKeyPressed(GLFW_KEY_R)) {
	    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
	        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	        enableLooking();
	    }
	    else {
	        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	        disableLooking();
	    }
	}

	double currentMouseX, currentMouseY;
	glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
	
	if (mIsLookingEnabled) {
		if (!mIsFirstMouseMovement) {
			double dX = currentMouseX - mPrevMouseX;
			double dY = currentMouseY - mPrevMouseY;
			mYawDeg += dX * mSensitivity;
			mPitchDeg -= dY * mSensitivity;

			if (mPitchDeg > 85) {
				mPitchDeg = 85;
			}
			if (mPitchDeg < -85) {
				mPitchDeg = -85;
			}
		}
		else {
			mIsFirstMouseMovement = false;
		}
		mPrevMouseX = currentMouseX;
		mPrevMouseY = currentMouseY;
	}

	glm::vec3 flatForward{ getForward() };
	flatForward.y = 0;
	flatForward = glm::normalize(flatForward);
	glm::vec3 flatRight{ -flatForward.z, 0, flatForward.x };

	glm::vec3 moveDirection{ 0, 0, 0 };
	if (inputs.checkKeyDown(GLFW_KEY_W)) {
		moveDirection += flatForward;
	}
	if (inputs.checkKeyDown(GLFW_KEY_S)) {
		moveDirection -= flatForward;
	}
	if (inputs.checkKeyDown(GLFW_KEY_D)) {
		moveDirection += flatRight;
	}
	if (inputs.checkKeyDown(GLFW_KEY_A)) {
		moveDirection -= flatRight;
	}
	if (inputs.checkKeyDown(GLFW_KEY_SPACE)) {
		moveDirection += glm::vec3{ 0, 1, 0 };
	}
	if (inputs.checkKeyDown(GLFW_KEY_C)) {
		moveDirection -= glm::vec3{ 0, 1, 0 };
	}

	if (glm::length(moveDirection) != 0) {
		mPosition += glm::normalize(moveDirection) * deltaTime;
	}
}

glm::vec3 Camera::getForward() const {
	glm::vec3 forward;
	forward.x = (float) (glm::sin(glm::radians(mYawDeg)) * glm::cos(glm::radians(mPitchDeg)));
	forward.y = (float) (glm::sin(glm::radians(mPitchDeg)));
	forward.z = (float)(-glm::cos(glm::radians(mYawDeg)) * glm::cos(glm::radians(mPitchDeg)));
	return forward;
}

glm::mat4 Camera::getViewMatrix() const {
	glm::vec3 forward{ getForward() };
	return glm::lookAt(mPosition, mPosition + forward, glm::vec3{ 0, 1, 0 });
}

glm::mat4 Camera::getProjectionMatrix() const {
	return glm::perspective(glm::radians((float)mHorizontalFOVDeg), (float)mScreenWidth / mScreenHeight, 0.1f, 1000.0f);
}