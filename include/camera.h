#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "inputs.h"
#include "nlohmann/json.hpp"
#include "camerarenderdata.h"

struct GLFWwindow;

class Camera {
public:
	Camera(const glm::vec3& position, double horizontalFOVDeg, double lookSensitivity);
	void loadJSONData(const JSON& json);
	void update(GLFWwindow* window, const Inputs& inputs, float deltaTime);
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(float aspectRatio) const;
	glm::vec3 getForward() const;
	glm::vec3 getPosition() const { return mPosition; }
	void disableLooking() { mIsLookingEnabled = false; }
	void enableLooking() { mIsLookingEnabled = true; mIsFirstMouseMovement = true; }
	bool isCursorFree() const { return !mIsLookingEnabled; }
	CameraRenderData getCameraRenderData(float aspectRatio) const { return CameraRenderData{ getViewMatrix(), getProjectionMatrix(aspectRatio), mPosition }; }

	JSON toJSON() const;

private:
	glm::vec3 mPosition;
	double mPitchDeg{ 0 }; // 0, 0 represents world forward / -z
	double mYawDeg{ 0 };
	double mHorizontalFOVDeg;
	double mSensitivity;
	double mPrevMouseX;
	double mPrevMouseY;
	bool mIsLookingEnabled = true;
	bool mIsFirstMouseMovement{ true };
};

#endif