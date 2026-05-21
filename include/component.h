#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "componenttypes.h"
#include "nlohmann/json.hpp"
#include <memory>
#include <optional>

class Component {
public:
	Component(ComponentTypes::Type componentType) : mComponentType{ componentType } {}

	/// <summary>
	/// Creates a component on the heap (you must handle this memory yourself)
	/// </summary>
	/// <param name="metaJSON">JSON containing a valid 'name' string for a component type and a 'data' object to initialize it with</param>
	/// <param name="location">An already allocated and managed heap location. If this argument is provided, the newly created object will be created in that location</param>
	/// <returns></returns>
	static Component* heapFromMetaJSON(const JSON& metaJSON, std::optional<void*> location = std::nullopt) {
		ComponentTypes::Type type{ ComponentTypes::nameToType[metaJSON.at("name")] };
		return (ComponentTypes::heapFromJSON[(int)type])(metaJSON.at("data"), location);
	}
	virtual JSON copyInitialMetaJSON() const { return mInitialMetaJSON; }
	virtual void renderUIProperties() = 0;
	//virtual JSON toMetaJSON() = 0;
	virtual ComponentTypes::Type getComponentType() const { return mComponentType; };
	virtual ~Component() {}

protected:
	ComponentTypes::Type mComponentType;
	JSON mInitialMetaJSON;
};

#endif

/*
The componentTypes::fromJSON should take in an optional placement pointer and call new then return the unowned pointer
*/