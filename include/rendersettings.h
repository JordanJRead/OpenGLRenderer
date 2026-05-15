#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include "glm/glm.hpp"
#include "OpenGLObjects/BUF.h"

class RenderSettings {
public:
	RenderSettings();
	glm::vec3 mHighlightColour{ 0.2f, 0.2f, 0.2f };
	float mPointLightRenderScale{ 0.3f };
	bool mShouldRenderPointLights{ true };
	void updateGPU();

private:
	glm::vec3 mPrevHighlightColour;
	float mPrevPointLightRenderScale;

	BUF mBuffer;
	void sendToGPU();
};

#endif