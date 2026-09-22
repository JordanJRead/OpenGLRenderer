#include "component.hpp"

Component::Component(std::string_view componentName, const JSON& json,
                     bool isScript)
: mComponentName{ componentName }
, mIsScript{ isScript }
, mUIProperties{ json } {
}

void Component::renderUIProperties() {
    ImGui::SeparatorText(mComponentName.c_str());
    mUIProperties.renderUI();
    ImGui::PushID(this);
    if (ImGui::Button("Update")) {
        ImGui::PopID();
        readProperties(mUIProperties);
    } else {
        ImGui::PopID();
    }
}

JSON Component::toJSON() const {
    return mUIProperties.toJSON();
}

std::string_view Component::getName() const {
    return mComponentName;
}

bool Component::isScript() const {
    return mIsScript;
}

void Component::readOwnProperties() {
    readProperties(mUIProperties);
}

Component::~Component() {
}
