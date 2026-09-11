#ifndef SCENE_OBJECT_COMPONENT_H
#define SCENE_OBJECT_COMPONENT_H

#include "editableproperties.hpp"
#include "imgui/imgui.h"
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

    void updateProperties();

    virtual ~Component();

protected:
    virtual void setJSONAndCreate(const JSON& json) final;
    bool         mIsScript{ false };

private:
    EditableProperties mInitialProperties;
    EditableProperties mEditableProperties;
    std::string        mComponentName;

    virtual void create(EditableProperties& properties) = 0;
};

#endif