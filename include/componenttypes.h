#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <array>
#include <string>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <unordered_map>

using JSON = nlohmann::json;
class Component;

namespace ComponentTypes {
	enum Type {
		model,
		pointLight,
		max
	};

	extern std::array<std::string, (int)Type::max> names;
	extern std::array<std::function<std::unique_ptr<Component>(const JSON& json)>, (int)Type::max> fromJSON;
	extern std::unordered_map<std::string, ComponentTypes::Type> nameToType;
}

#endif