#include "componentdropdown.h"
#include "imgui.h"

void ComponentDropdown::renderUI() {
	if (ImGui::BeginCombo("Add Component", getDisplayText(mSelectedType).data())) {
		for (int i{ 0 }; i <= ComponentTypes::max; ++i) {
			bool isSelected{ i == mSelectedType };
			if (ImGui::Selectable(getDisplayText((ComponentTypes::Type)i).data(), isSelected)) {
				mSelectedType = (ComponentTypes::Type)i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
}

std::string_view ComponentDropdown::getDisplayText(ComponentTypes::Type type) const {
	if (type == ComponentTypes::max) {
		return "...";
	}
	return ComponentTypes::names[(int)type];
}