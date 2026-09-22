#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

using UIPropertyVariant = std::variant<bool, glm::vec3, glm::vec3, double, int,
                                       std::string, glm::vec3, glm::vec4>;

class UIProperty {
public:
    enum class Type {
        bool_type,
        colour_type,
        direction_type,
        double_type,
        int_type,
        string_type,
        vec3_type,
        vec4_type
    };

    template <Type type>
    struct TypeTag {};

    template <typename T, Type TypeEnum>
    UIProperty(const T& value, TypeTag<TypeEnum>) {
        constexpr int index = static_cast<size_t>(TypeEnum);
        using UnderlyingType
          = std::variant_alternative_t<index, UIPropertyVariant>;
        static_assert(std::is_same_v<T, UnderlyingType>,
                      "UIPROPERTY::UIPROPERTY ERROR: Invalid type "
                      "of value given to constructor");
        mValue.emplace<index>(value);
    }

    Type getType() const {
        return (Type)(mValue.index());
    }

    template <Type TypeEnum>
    std::variant_alternative_t<static_cast<size_t>(TypeEnum),
                               UIPropertyVariant>&
    get() {
        return std::get<static_cast<size_t>(TypeEnum)>(mValue);
    }
    template <Type TypeEnum>
    const std::variant_alternative_t<static_cast<size_t>(TypeEnum),
                                     UIPropertyVariant>&
    get() const {
        return std::get<static_cast<size_t>(TypeEnum)>(mValue);
    }

private:
    UIPropertyVariant mValue;

    void checkType(Type type) const {
        if (getType() != type)
            throw std::runtime_error("Invalid property type access!");
    }
};

#endif
