#include "uipropertybased.hpp"
#include "imgui/imgui.h"

UIPropertyBased::UIPropertyBased(const JSON& propertyJSON)
: mUIProperties{ propertyJSON } {
}

void UIPropertyBased::readOwnProperties() {
    readProperties(mUIProperties);
}

JSON UIPropertyBased::toJSON() const {
    return mUIProperties.toJSON();
}

void UIPropertyBased::renderUIProperties() {
    mUIProperties.renderUI();
    ImGui::PushID(this);
    if (ImGui::Button("Update")) {
        ImGui::PopID();
        readProperties(mUIProperties);
    } else {
        ImGui::PopID();
    }
}