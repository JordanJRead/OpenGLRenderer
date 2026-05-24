#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "componenttypes.h"
#include <nlohmann/json.hpp>
#include "editableproperties.h"
#include "imgui.h"

class Component {
public:
	Component(ComponentTypes::Type componentType) : mComponentType{ componentType } {}

	virtual void renderUIProperties() {
		ImGui::SeparatorText(ComponentTypes::names[(int)mComponentType].data());
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

	virtual ComponentTypes::Type getComponentType() const { return mComponentType; };
	virtual ~Component() {}

protected:
	virtual void setJSONAndCreate(const JSON& json) final {
		mInitialProperties.create(json);
		create(mInitialProperties);
		mInitialProperties.removeUnused();
		mEditableProperties = mInitialProperties;
	}

private:
	ComponentTypes::Type mComponentType;
	EditableProperties mInitialProperties;
	EditableProperties mEditableProperties;

	virtual void create(EditableProperties& properties) = 0;
};

#endif