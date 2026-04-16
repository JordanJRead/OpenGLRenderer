#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include "glm/glm.hpp"

struct RenderSettings {
	bool mShouldRenderPointLights{ true };
	glm::vec3 mPointLightRenderScale{ 0.3f, 0.3f, 0.3f };
};

#endif