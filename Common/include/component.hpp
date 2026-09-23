#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "uipropertybased.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

class Component : public UIPropertyBased {
public:
    Component(std::string_view componentName, const JSON& json, bool isScript);

    std::string_view getName() const;

    bool isScript() const;

    void renderUIProperties() override;

    virtual ~Component();

private:
    std::string mComponentName;
    bool        mIsScript{ false };
};

#endif