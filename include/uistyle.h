#ifndef UI_STYLE_H
#define UI_STYLE_H

#include "changeable.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include <vector>
#include <string_view>
#include "nlohmann/json.hpp"

class UIStyle {
public:
	void renderUIWindow();
	JSON toJSON() const;
	void fromJSON(const JSON& json);

private:
	class ColourItem {
	public:
		ColourItem(std::string_view name, ImGuiCol_ imGuiEnum);

		Changeable<glm::vec4> mData;
		std::string_view mDisplayName;
		ImGuiCol_ mImGuiEnum;
	};

	std::vector< ColourItem> mColours{
		ColourItem{"Text colour", ImGuiCol_Text},
		ColourItem{"Window BG colour", ImGuiCol_WindowBg},
		ColourItem{"Window border", ImGuiCol_Border},
		ColourItem{"Item BG", ImGuiCol_FrameBg},
		ColourItem{"Item BG hover", ImGuiCol_FrameBgHovered},
		ColourItem{"Item BG active", ImGuiCol_FrameBgActive},
		ColourItem{"Title BG", ImGuiCol_TitleBg},
		ColourItem{"Title BG active", ImGuiCol_TitleBgActive},
		ColourItem{"Title BG collapsed", ImGuiCol_TitleBgCollapsed},
	};
};

#endif