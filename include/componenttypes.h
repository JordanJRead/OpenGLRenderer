#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <array>
#include <string>

namespace ComponentTypes {
	enum Type {
		model,
		pointLight,
		max
	};

	extern std::array<std::string, (int)Type::max> names;
}

#endif