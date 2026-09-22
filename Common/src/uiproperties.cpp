#include "uiproperties.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "jsonhelpers.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

using Type = UIProperty::Type;

template <Type TypeEnum>
using TypeTag = UIProperty::TypeTag<TypeEnum>;

UIProperties::UIProperties(const JSON& json) {
    mProperties.clear();
    mAccessedProperties.clear();
    if (json.type() != JSON::value_t::object) {
        throw std::runtime_error("UIPROPERTIES::UIPROPERTIES "
                                 "ERROR: json must be object type");
    }

    // data has 'type' and 'value'
    for (auto& [key, data] : json.items()) {
        UIProperty::Type type{ data.at("type") };
        JSON             valueJSON = data.at("value");
        switch (type) {
        case UIProperty::Type::bool_type:
            mProperties.insert({
              key, UIProperty{ valueJSON.m_data.m_value.boolean,
                              TypeTag<Type::bool_type>{} }
            });
            break;

        case UIProperty::Type::colour_type:
            mProperties.insert({
              key, UIProperty{ JSONHelpers::toVec3(valueJSON),
                              TypeTag<Type::colour_type>{} }
            });
            break;

        case UIProperty::Type::direction_type:
            mProperties.try_emplace(key, JSONHelpers::toVec3(valueJSON),
                                    TypeTag<Type::direction_type>{});
            break;

        case UIProperty::Type::double_type:
            mProperties.try_emplace(key, valueJSON.m_data.m_value.number_float,
                                    TypeTag<Type::double_type>{});
            break;

        case UIProperty::Type::int_type:
            mProperties.try_emplace(
              key, (int)valueJSON.m_data.m_value.number_integer,
              TypeTag<Type::int_type>{});
            break;

        case UIProperty::Type::string_type:
            mProperties.try_emplace(key, *valueJSON.m_data.m_value.string,
                                    TypeTag<Type::string_type>{});
            break;

        case UIProperty::Type::vec3_type:
            mProperties.try_emplace(key, JSONHelpers::toVec3(json),
                                    TypeTag<Type::vec3_type>{});
            break;

        case UIProperty::Type::vec4_type:
            mProperties.try_emplace(key, JSONHelpers::toVec4(json),
                                    TypeTag<Type::vec4_type>{});
            break;
        }
    }
}

JSON UIProperties::toJSON() const {
    JSON json = JSON::object();

    for (const auto& [name, property] : mProperties) {
        json[name]         = JSON::object();
        json[name]["type"] = (int)property.getType();
        switch (property.getType()) {
        case UIProperty::Type::bool_type:
            json[name]["value"] = property.get<UIProperty::Type::bool_type>();
            break;

        case UIProperty::Type::colour_type:
            json[name]["value"] = JSONHelpers::fromVec3(
              property.get<UIProperty::Type::colour_type>());
            break;

        case UIProperty::Type::direction_type:
            json[name]["value"] = JSONHelpers::fromVec3(
              property.get<UIProperty::Type::direction_type>());
            break;

        case UIProperty::Type::double_type:
            json[name]["value"] = property.get<UIProperty::Type::double_type>();
            break;

        case UIProperty::Type::int_type:
            json[name]["value"] = property.get<UIProperty::Type::int_type>();
            break;

        case UIProperty::Type::string_type:
            json[name]["value"] = property.get<UIProperty::Type::string_type>();
            break;

        case UIProperty::Type::vec3_type:
            json[name]["value"] = JSONHelpers::fromVec3(
              property.get<UIProperty::Type::vec3_type>());
            break;

        case UIProperty::Type::vec4_type:
            json[name]["value"] = JSONHelpers::fromVec4(
              property.get<UIProperty::Type::vec4_type>());
            break;
        }
    }

    return json;
}

void UIProperties::renderUI() {
    ImGui::PushID(this);
    typedef UIProperty::Type Type;
    for (auto& pair : mProperties) {

        switch (pair.second.getType()) {
        case Type::bool_type:
            ImGui::Checkbox(pair.first.data(),
                            &pair.second.get<Type::bool_type>());
            break;

        case Type::colour_type:
            ImGui::ColorEdit3(pair.first.data(),
                              (float*)&pair.second.get<Type::colour_type>());
            break;

        case Type::direction_type:
            ImGui::DragFloat3(pair.first.data(),
                              (float*)&pair.second.get<Type::direction_type>());
            if (glm::length(pair.second.get<Type::direction_type>()) != 0)
                pair.second.get<Type::direction_type>()
                  = glm::normalize(pair.second.get<Type::direction_type>());
            break;

        case Type::double_type:
            ImGui::DragScalar(pair.first.data(), ImGuiDataType_Double,
                              &pair.second.get<Type::double_type>());
            break;

        case Type::int_type:
            ImGui::DragInt(pair.first.data(),
                           &pair.second.get<Type::int_type>());
            break;

        case Type::string_type:
            ImGui::InputText(pair.first.data(),
                             &pair.second.get<Type::string_type>());
            break;

        case Type::vec3_type:
            ImGui::DragFloat3(pair.first.data(),
                              (float*)&pair.second.get<Type::vec3_type>());
            break;

        case Type::vec4_type:
            ImGui::DragFloat4(pair.first.data(),
                              (float*)&pair.second.get<Type::vec4_type>());
            break;
        }
    }
    ImGui::PopID();
}