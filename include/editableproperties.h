#ifndef CONSTRUCTABLE_PROPERTIES
#define CONSTRUCTABLE_PROPERTIES

#include <map>
#include "nlohmann/json.hpp"
#include <string>
#include <string_view>
#include "editableproperty.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <cassert>
#include "jsonhelpers.h"

using Type = EditableProperty::Type;

template <Type TypeEnum>
using TypeTag = EditableProperty::TypeTag<TypeEnum>;

class EditableProperties {
public:
	void create(const JSON& json) {
		mProperties.clear();
		if (json.type() != JSON::value_t::object) {
			throw std::exception("EDITABLEPROPERTIES::EDITABLEPROPERTIES ERROR: json must be object type");
		}

		// data has 'type' and 'value'
		for (auto& [key, data] : json.items()) {
			EditableProperty::Type type{ data.at("type") };
			JSON valueJSON = data.at("value");
			switch (type) {
			case EditableProperty::Type::bool_type:
				mProperties.insert({ key, EditableProperty{ valueJSON.m_data.m_value.boolean, TypeTag<Type::bool_type>{} } });
				break;

			case EditableProperty::Type::colour_type:
				mProperties.insert({ key, EditableProperty{ JSONHelpers::toVec3(valueJSON), TypeTag<Type::colour_type>{} } });
				break;

			case EditableProperty::Type::direction_type:
				mProperties.try_emplace(key, JSONHelpers::toVec3(valueJSON), TypeTag<Type::direction_type>{});
				break;

			case EditableProperty::Type::double_type:
				mProperties.try_emplace(key, valueJSON.m_data.m_value.number_float, TypeTag<Type::double_type>{});
				break;

			case EditableProperty::Type::int_type:
				mProperties.try_emplace(key, (int)valueJSON.m_data.m_value.number_integer, TypeTag<Type::int_type>{});
				break;

			case EditableProperty::Type::string_type:
				mProperties.try_emplace(key, *valueJSON.m_data.m_value.string, TypeTag<Type::string_type>{});
				break;

			case EditableProperty::Type::vec3_type:
				mProperties.try_emplace(key, JSONHelpers::toVec3(json), TypeTag<Type::vec3_type>{});
				break;

			case EditableProperty::Type::vec4_type:
				mProperties.try_emplace(key, JSONHelpers::toVec4(json), TypeTag<Type::vec4_type>{});
				break;
			}
		}
	}

	JSON toJSON() {
		JSON json = JSON::object();

		for (const auto& [name, property] : mProperties) {
			json["name"] = JSON::object();
			json["name"]["type"] = (int)property.getType();
			switch (property.getType()) {
			case EditableProperty::Type::bool_type:
				json[name]["value"] = property.get<EditableProperty::Type::bool_type>();
				break;

			case EditableProperty::Type::colour_type:
				json[name]["value"] = JSONHelpers::fromVec3(property.get<EditableProperty::Type::colour_type>());
				break;

			case EditableProperty::Type::direction_type:
				json[name]["value"] = JSONHelpers::fromVec3(property.get<EditableProperty::Type::direction_type>());
				break;

			case EditableProperty::Type::double_type:
				json[name]["value"] = property.get<EditableProperty::Type::double_type>();
				break;

			case EditableProperty::Type::int_type:
				json[name]["value"] = property.get<EditableProperty::Type::int_type>();
				break;

			case EditableProperty::Type::string_type:
				json[name]["value"] = property.get<EditableProperty::Type::string_type>();
				break;

			case EditableProperty::Type::vec3_type:
				json[name]["value"] = JSONHelpers::fromVec3(property.get<EditableProperty::Type::vec3_type>());
				break;

			case EditableProperty::Type::vec4_type:
				json[name]["value"] = JSONHelpers::fromVec4(property.get<EditableProperty::Type::vec4_type>());
				break;
			}
		}

		return json;
	}

	const EditableProperty* const get(const std::string& propertyName) const {
		if (mProperties.contains(propertyName)) {
			return &mProperties.at(propertyName);
		}
		return nullptr;
	}

	template <typename T>
	void add(std::string_view propertyName, const T& value, EditableProperty::Type type) {
		mProperties.insert({ propertyName, EditableProperty{ value, type } });
	}

	void renderUI() {
		typedef EditableProperty::Type Type;
		for (auto& pair : mProperties) {

			switch (pair.second.getType()) {
			case Type::bool_type:
				ImGui::Checkbox(pair.first.data(), &pair.second.get<Type::bool_type>());
				break;

			case Type::colour_type:
				ImGui::ColorEdit3(pair.first.data(), (float*)&pair.second.get<Type::colour_type>());
				break;

			case Type::direction_type:
				ImGui::DragFloat3(pair.first.data(), (float*)&pair.second.get<Type::direction_type>());
				if (glm::length(pair.second.get<Type::direction_type>()) != 0)
					pair.second.get<Type::direction_type>() = glm::normalize(pair.second.get<Type::direction_type>());
				break;

			case Type::double_type:
				ImGui::DragScalar(pair.first.data(), ImGuiDataType_Double, &pair.second.get<Type::double_type>());
				break;

			case Type::int_type:
				ImGui::DragInt(pair.first.data(), &pair.second.get<Type::int_type>());
				break;

			case Type::string_type:
				ImGui::InputText(pair.first.data(), &pair.second.get<Type::string_type>());
				break;

			case Type::vec3_type:
				ImGui::DragFloat3(pair.first.data(), (float*) & pair.second.get<Type::vec3_type>());
				break;

			case Type::vec4_type:
				ImGui::DragFloat4(pair.first.data(), (float*)&pair.second.get<Type::vec4_type>());
				break;
			}
		}
	}

	std::map<std::string, EditableProperty> mProperties;
};

#endif