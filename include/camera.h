#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

struct GLFWwindow;

class Camera {
public:
	Camera(const glm::vec3& position, double horizontalFOVDeg, double lookSensitivity);
	void update(GLFWwindow* window, float deltaTime);
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const;
	glm::vec3 getForward() const;
	glm::vec3 getPosition() const { return mPosition; }

private:
	glm::vec3 mPosition;
	double mPitchDeg{ 0 }; // 0, 0 represents world forward / -z
	double mYawDeg{ 0 };
	double mHorizontalFOVDeg;
	double mLookSensitivity;
	double mPrevMouseX;
	double mPrevMouseY;
	bool mIsFirstUpdate{ true };
};

#endif