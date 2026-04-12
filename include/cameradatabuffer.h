#ifndef CAMERA_DATA_BUFFER_H
#define CAMERA_DATA_BUFFER_H

#include "OpenGLObjects/BUF.h"
#include "camera.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

class CameraDataBuffer {
public:
	CameraDataBuffer() {
		glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, mBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 0, 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void update(const Camera& camera) {
		glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);

		struct CameraData {
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
			glm::vec3 cameraPos;
		} data;
		data.viewMatrix = camera.getViewMatrix();
		data.projectionMatrix = camera.getProjectionMatrix();
		data.cameraPos = camera.getPosition();

		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &data, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

private:
	BUF mBuffer;
};

#endif