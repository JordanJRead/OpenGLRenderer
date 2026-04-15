#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "componenttypes.h"

class Component {
public:
	Component(ComponentTypes::Type componentType) : mComponentType{ componentType } {}
	virtual void renderUIProperties() = 0;
	virtual ComponentTypes::Type getComponentType() const { return mComponentType; };
	virtual ~Component() {}

protected:
	ComponentTypes::Type mComponentType;
};

#endif