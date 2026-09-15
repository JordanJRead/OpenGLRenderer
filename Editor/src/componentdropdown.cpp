#include "componentdropdown.hpp"
#include "imgui/imgui.h"
#include "componentmanager.hpp"

void ComponentDropdown::renderUI() {
	if (ImGui::BeginCombo("Add Component", mSelectedTypeName.c_str())) {
		for (const std::string& name : ComponentManager::instance()) {
			bool isSelected{ mSelectedTypeName == name };
			if (ImGui::Selectable(name.c_str(), isSelected)) {
				mSelectedTypeName = name;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
}
