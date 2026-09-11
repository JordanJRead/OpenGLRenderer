#include "component.hpp"

Component::Component(std::string_view componentName)
: mComponentName{ componentName } {
}

void Component::renderUIProperties() {
    ImGui::SeparatorText(mComponentName.c_str());
    mEditableProperties.renderUI();
    ImGui::PushID(this);
    if (ImGui::Button("Update")) {
        ImGui::PopID();
        setJSONAndCreate(mEditableProperties.toJSON());
    } else {
        ImGui::PopID();
    }
}

JSON Component::toJSON() const {
    return mInitialProperties.toJSON();
}

std::string_view Component::getName() const {
    return mComponentName;
}

bool Component::isScript() const {
    return mIsScript;
}

void Component::updateProperties() {
    setJSONAndCreate(mEditableProperties.toJSON());
}

Component::~Component() {
}

void Component::setJSONAndCreate(const JSON& json) {
    mInitialProperties.create(json);
    create(mInitialProperties);
    mInitialProperties.removeUnused();
    mEditableProperties = mInitialProperties;
}
