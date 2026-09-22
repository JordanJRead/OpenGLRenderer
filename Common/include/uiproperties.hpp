#ifndef CONSTRUCTABLE_PROPERTIES
#define CONSTRUCTABLE_PROPERTIES

#include "nlohmann/json.hpp"
#include "uiproperty.hpp"
#include <map>
#include <string>
#include <string_view>
#include <vector>

class UIProperties {
public:
    void create(const JSON& json);
    JSON toJSON() const;

    template <UIProperty::Type TypeEnum>
#define RETURN_TYPE \
    std::variant_alternative_t<static_cast<size_t>(TypeEnum), UIPropertyVariant>
    RETURN_TYPE& getOrCreate(const std::string& propertyName) {
        mAccessedProperties.push_back(propertyName);
        if (!mProperties.contains(propertyName)) {
            add(propertyName, RETURN_TYPE{}, UIProperty::TypeTag<TypeEnum>{});
        }
        return mProperties.at(propertyName).get<TypeEnum>();
    }

    void renderUI();
    void removeUnused() {
        for (auto it = mProperties.begin(); it != mProperties.end();) {
            if (std::find(mAccessedProperties.begin(),
                          mAccessedProperties.end(), it->first)
                == mAccessedProperties.end()) {
                it = mProperties.erase(it);
            } else {
                ++it;
            }
        }
        mAccessedProperties.clear();
    }

private:
    template <typename T, UIProperty::Type TypeEnum>
    void add(const std::string& propertyName, const T& value,
             UIProperty::TypeTag<TypeEnum> typeTag) {
        mProperties.insert({
          propertyName, UIProperty{ value, typeTag }
        });
    }

    std::map<std::string, UIProperty> mProperties;
    std::vector<std::string>          mAccessedProperties;
};

#endif