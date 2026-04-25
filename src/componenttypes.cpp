#include "componenttypes.h"
#include "model.h"
#include "pointlight.h"

namespace ComponentTypes {
	std::array<std::string, (int)Type::max> names{
		"Model",
		"Point Light"
	};

	std::array<std::function<std::unique_ptr<Component>(const JSON& json)>, (int)Type::max> fromJSON{
		[](const JSON& json) { return Model::fromJSON(json); },
		[](const JSON& json) { return PointLight::fromJSON(json); }
	};

	std::unordered_map<std::string, ComponentTypes::Type> nameToType{
		{ "Model", Type::model },
		{ "Point Light", Type::pointLight }
	};
}