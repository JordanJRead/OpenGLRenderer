#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include "glm/glm.hpp"
#include "OpenGLObjects/BUF.h"
#include "nlohmann/json.hpp"
#include "jsonhelpers.h"

class RenderSettings {
public:
	glm::vec3 mHighlightColour{ 0.2f, 0.2f, 0.2f };
	float mPointLightRenderScale{ 0.3f };
	bool mShouldRenderPointLights{ true };

	bool operator==(const RenderSettings&) const = default;
	JSON toJSON() const {
		JSON json;
		json["highlightColour"] = JSONHelpers::fromVec3(mHighlightColour);
		json["pointLightRenderScale"] = mPointLightRenderScale;
		json["shouldRenderPointLights"] = mShouldRenderPointLights;
		return json;
	}
	void fromJSON(const JSON& json) {
		mHighlightColour = JSONHelpers::toVec3(json["highlightColour"]);
		mPointLightRenderScale = json["pointLightRenderScale"];
		mShouldRenderPointLights = json["shouldRenderPointLights"];
	}
};

#endif