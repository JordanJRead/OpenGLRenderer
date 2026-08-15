#include "pointlight.hpp"
#include <stdexcept>
#include <array>
#include "jsonhelpers.hpp"

PointLight::PointLight(const JSON& json) : Component{ "Point Light" } {
	setJSONAndCreate(json);
}

PointLight::PointLight() : Component{ "Point Light" } {
	setJSONAndCreate(JSON::object());
}

void PointLight::create(EditableProperties& properties) {
	mColour = properties.getOrCreate<EditableProperty::Type::colour_type>("colour");
}