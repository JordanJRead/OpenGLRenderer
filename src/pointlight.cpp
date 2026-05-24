#include "pointlight.h"
#include <stdexcept>
#include <array>
#include "jsonhelpers.h"

PointLight::PointLight(const JSON& json) : Component{ PointLight::getComponentType() } {
	setJSONAndCreate(json);
}

void PointLight::create(const EditableProperties& properties) {
	mColour = properties.get("colour")->get<EditableProperty::Type::colour_type>();
}