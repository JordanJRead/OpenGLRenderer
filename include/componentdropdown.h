#ifndef COMPONENT_DROPDOWN_H
#define COMPONENT_DROPDOWN_H

#include "componenttypes.h"
#include <string_view>

class ComponentDropdown {
public:
	ComponentTypes::Type getCurrentType() { return mSelectedType; }
	void reset() { mSelectedType = ComponentTypes::max; }
	void renderUI();

private:
	ComponentTypes::Type mSelectedType{ ComponentTypes::max };

	std::string_view getDisplayText(ComponentTypes::Type type) const;
};

#endif