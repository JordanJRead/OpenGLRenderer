#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "imgui/imgui.h"
#include "uiproperties.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

class Component {
public:
    Component(std::string_view componentName);

    virtual void renderUIProperties();

    JSON toJSON() const;

    std::string_view getName() const;

    bool isScript() const;

    virtual ~Component();

protected:
    virtual void setJSONAndCreate(const JSON& json) final;
    bool         mIsScript{ false };

private:
    UIProperties mInitialProperties;
    UIProperties mEditableProperties;
    std::string  mComponentName;

    virtual void create(UIProperties& properties) = 0;
};

#endif