#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "imgui/imgui.h"
#include "uiproperties.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

class Component {
public:
    Component(std::string_view componentName, const JSON& json, bool isScript);

    virtual void renderUIProperties();

    JSON toJSON() const;

    std::string_view getName() const;

    bool isScript() const;

    virtual ~Component();

    void readOwnProperties();

private:
    UIProperties mUIProperties;
    std::string  mComponentName;
    bool         mIsScript{ false };

    virtual void readProperties(UIProperties& properties) = 0;
};

#endif