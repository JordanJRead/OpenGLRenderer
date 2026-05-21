#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <array>
#include <string>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <optional>

class Component;

namespace ComponentTypes {
	enum Type {
		model,
		pointLight,
		max
	};

	extern std::array<std::string, (int)Type::max> names;
	extern std::array<std::function<Component* (const JSON& json, std::optional<void*> location)>, (int)Type::max> heapFromJSON;
	extern std::unordered_map<std::string, ComponentTypes::Type> nameToType;
}

#endif