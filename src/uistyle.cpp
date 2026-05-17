#include "uistyle.h"

#include "changeable.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "jsonhelpers.h"

#define CONVERT(vec, type) ( type { ((vec).x), ((vec).y), ((vec).z), ((vec).w) })

void UIStyle::renderUIWindow() {
	ImGui::Begin("Editor Style");

	for (ColourItem& item : mColours) {
		ImGui::ColorEdit4(item.mDisplayName.data(), (float*)&item.mData);
		if (item.mData.hasChanged()) {
			(ImGui::GetStyle()).Colors[item.mImGuiEnum] = CONVERT(item.mData.mValue, ImVec4);
		}
	}

	ImGui::End();
}

JSON UIStyle::toJSON() const {
	JSON json;
	for (const ColourItem& item : mColours) {
		json[item.mDisplayName] = JSONHelpers::fromVec4(item.mData.mValue);
	}
	return json;
}

void UIStyle::fromJSON(const JSON& json) {
	for (ColourItem& item : mColours) {
		if (json.contains(item.mDisplayName))
			item.mData.mValue = JSONHelpers::toVec4(json.at(item.mDisplayName));
	}
}

UIStyle::ColourItem::ColourItem(std::string_view name, ImGuiCol_ imGuiEnum) : mDisplayName{ name }, mImGuiEnum{ imGuiEnum } {
	mData.mValue = CONVERT(ImGui::GetStyle().Colors[mImGuiEnum], glm::vec4);
}