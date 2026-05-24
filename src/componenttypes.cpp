#include "componenttypes.h"
#include "model.h"
#include "pointlight.h"

namespace ComponentTypes {
	std::array<std::string, (int)Type::max> names{
		"Model",
		"Point Light"
	};

	std::array<std::function<std::unique_ptr<Component>(const JSON& json)>, (int)Type::max> createFromJSON{
		[](const JSON& json) { return std::make_unique<Model>(json); },
		[](const JSON& json) { return std::make_unique<PointLight>(json); }
	};

	std::array<std::function<std::unique_ptr<Component>()>, (int)Type::max> createEmpty{
		[]() { return std::make_unique<Model>(); },
		[]() { return std::make_unique<PointLight>(); }
	};

	std::unordered_map<std::string, ComponentTypes::Type> nameToType{
		{ "Model", Type::model },
		{ "Point Light", Type::pointLight }
	};
}