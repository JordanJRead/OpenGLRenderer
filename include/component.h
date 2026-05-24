#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "componenttypes.h"
#include <nlohmann/json.hpp>
#include "editableproperties.h"

class Component {
public:
	Component(ComponentTypes::Type componentType) : mComponentType{ componentType } {}

	virtual void renderUIProperties() {
		mEditableProperties.renderUI();
	}

	JSON toJSON() {
		return mInitialProperties.toJSON();
	}

	virtual ComponentTypes::Type getComponentType() const { return mComponentType; };
	virtual ~Component() {}

protected:
	virtual void setJSONAndCreate(const JSON& json) final {
		mInitialProperties.create(json);
		mEditableProperties.create(json);
		create(mInitialProperties);
	}

private:
	ComponentTypes::Type mComponentType;
	EditableProperties mInitialProperties;
	EditableProperties mEditableProperties;

	virtual void create(const EditableProperties& properties) = 0;
};

#endif