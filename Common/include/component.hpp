#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include <nlohmann/json.hpp>
#include "editableproperties.hpp"
#include "imgui/imgui.h"
#include <string>
#include <string_view>

class Component {
public:
	Component(std::string_view componentName) : mComponentName{ componentName }
	{}

	virtual void renderUIProperties() {
		ImGui::SeparatorText(mComponentName.c_str());
		mEditableProperties.renderUI();
		ImGui::PushID(this);
		if (ImGui::Button("Update")) {
			ImGui::PopID();
			setJSONAndCreate(mEditableProperties.toJSON());
		}
		else {
			ImGui::PopID();
		}
	}

	JSON toJSON() {
		return mInitialProperties.toJSON();
	}

	std::string_view getName() const { return mComponentName; }

	virtual ~Component() {}

protected:
	virtual void setJSONAndCreate(const JSON& json) final {
		mInitialProperties.create(json);
		create(mInitialProperties);
		mInitialProperties.removeUnused();
		mEditableProperties = mInitialProperties;
	}

private:
	EditableProperties mInitialProperties;
	EditableProperties mEditableProperties;
	std::string mComponentName;

	virtual void create(EditableProperties& properties) = 0;
};

#endif