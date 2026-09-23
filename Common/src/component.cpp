#include "component.hpp"
#include "imgui/imgui.h"

Component::Component(std::string_view componentName, const JSON& json,
                     bool isScript)
: mComponentName{ componentName }
, mIsScript{ isScript }
, UIPropertyBased{ json } {
}

std::string_view Component::getName() const {
    return mComponentName;
}

void Component::renderUIProperties() {
    ImGui::SeparatorText(mComponentName.c_str());
    UIPropertyBased::renderUIProperties();
}

bool Component::isScript() const {
    return mIsScript;
}

Component::~Component() {
}
