#include "pointlight.h"
#include <stdexcept>
#include <array>
#include "jsonhelpers.h"

PointLight::PointLight(const JSON& json) : Component{ PointLight::getComponentType() } {
	setJSONAndCreate(json);
}

PointLight::PointLight() : Component{ PointLight::getComponentType() } {
	setJSONAndCreate(JSON::object());
}

void PointLight::create(EditableProperties& properties) {
	mColour = properties.getOrCreate<EditableProperty::Type::colour_type>("colour");
}