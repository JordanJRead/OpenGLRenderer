#include "componenttypes.h"
#include "model.h"
#include "pointlight.h"

namespace ComponentTypes {
	std::array<std::string, (int)Type::max> names{
		"Model",
		"Point Light"
	};

	extern std::array<std::function<Component* (const JSON& json, std::optional<void*> location)>, (int)Type::max> heapFromJSON {
		[](const JSON& json, std::optional<void*> location) {
			if (location) { 
				return new (location.value()) Model(json);
			}
			else {
				return new Model(json);
			}
		},
		[](const JSON& json, std::optional<void*> location) {
			if (location) {
				return new (location.value()) PointLight(json);
			}
			else {
				return new PointLight(json);
			}
		}
	};

	std::unordered_map<std::string, ComponentTypes::Type> nameToType{
		{ "Model", Type::model },
		{ "Point Light", Type::pointLight }
	};
}