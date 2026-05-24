#ifndef CONSTRUCTABLE_PROPERTIES
#define CONSTRUCTABLE_PROPERTIES

#include <map>
#include "nlohmann/json.hpp"
#include <string>
#include <string_view>
#include "editableproperty.h"
#include <vector>

class EditableProperties {
public:
	void create(const JSON& json);
	JSON toJSON();

	template <EditableProperty::Type TypeEnum>
#define RETURN_TYPE std::variant_alternative_t<static_cast<size_t>(TypeEnum), EditableVariant_t>
	RETURN_TYPE& getOrCreate(const std::string& propertyName) {
		mAccessedProperties.push_back(propertyName);
		if (!mProperties.contains(propertyName)) {
			add(propertyName, RETURN_TYPE{}, EditableProperty::TypeTag<TypeEnum>{});
		}
		return mProperties.at(propertyName).get<TypeEnum>();
	}

	void renderUI();
	void removeUnused() {
		for (auto it = mProperties.begin(); it != mProperties.end();) {
			if (std::find(mAccessedProperties.begin(), mAccessedProperties.end(), it->first) == mAccessedProperties.end()) {
				it = mProperties.erase(it);
			}
			else {
				++it;
			}
		}
		mAccessedProperties.clear();
	}

private:

	template <typename T, EditableProperty::Type TypeEnum>
	void add(const std::string& propertyName, const T& value, EditableProperty::TypeTag<TypeEnum> typeTag) {
		mProperties.insert({ propertyName, EditableProperty{ value, typeTag } });
	}

	std::map<std::string, EditableProperty> mProperties;
	std::vector<std::string> mAccessedProperties;
};

#endif