#include "pointlight.hpp"
#include <stdexcept>
#include <array>
#include "jsonhelpers.hpp"

PointLight::PointLight(const JSON& json) : Component{ PointLight::staticGetComponentType() } {
	setJSONAndCreate(json);
}

PointLight::PointLight() : Component{ PointLight::staticGetComponentType() } {
	setJSONAndCreate(JSON::object());
}

void PointLight::create(EditableProperties& properties) {
	mColour = properties.getOrCreate<EditableProperty::Type::colour_type>("colour");
}