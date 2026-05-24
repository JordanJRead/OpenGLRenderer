#ifndef EDITABLE_PROPERTY_H
#define EDITABLE_PROPERTY_H

#include <memory>
#include "glm/glm.hpp"
#include <string>
#include "nlohmann/json.hpp"
#include <type_traits>
#include <variant>

using EditableVariant_t = std::variant<bool, glm::vec3, glm::vec3, double, int, std::string, glm::vec3, glm::vec4>;

class EditableProperty {
public:
	struct Metadata {
		float sensitivity;
	};

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
	EditableProperty(const T& value, TypeTag<TypeEnum>) {
		constexpr int index = static_cast<size_t>(TypeEnum);
		using UnderlyingType = std::variant_alternative_t<index, EditableVariant_t>;
		static_assert(std::is_same_v<T, UnderlyingType>, "EDITABLEPROPERTY::EDITABLEPROPERTY ERROR: Invalid type of value given to constructor");
		mValue.emplace<index>(value);
		mType = TypeEnum; // just call variant.getindex or something?
	}

	const Metadata& getMetadata() const { return mMetadata; }
	Type getType() const { return mType; }

	template <Type TypeEnum>
	std::variant_alternative_t<static_cast<size_t>(TypeEnum), EditableVariant_t>& get() {
		return std::get<static_cast<size_t>(TypeEnum)>(mValue);
	}
	template <Type TypeEnum>
	const std::variant_alternative_t<static_cast<size_t>(TypeEnum), EditableVariant_t>& get() const {
		return std::get<static_cast<size_t>(TypeEnum)>(mValue);
	}


private:
	Metadata mMetadata{}; // TODO actually use, make getOrCreate() in 'parent class' take in a value to set? Keep out of JSON then, or save?
	EditableVariant_t mValue;
	Type mType;

	void checkType(Type type) const {
		if (mType != type)
			throw std::exception("Invalid property type access!");
	}
};

#endif
