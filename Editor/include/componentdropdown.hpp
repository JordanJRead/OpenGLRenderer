#ifndef COMPONENT_DROPDOWN_H
#define COMPONENT_DROPDOWN_H

#include <string_view>
#include <string>

class ComponentDropdown {
public:
	std::string_view getCurrentType() { return mSelectedTypeName; }
	void reset() { mSelectedTypeName = ""; }
	void renderUI();

private:
	std::string mSelectedTypeName;
};

#endif