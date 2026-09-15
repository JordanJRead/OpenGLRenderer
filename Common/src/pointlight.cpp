#include "pointlight.hpp"
#include <stdexcept>
#include <array>
#include "jsonhelpers.hpp"

// TODO replace PointLight with type_name?
PointLight::PointLight(const JSON& json) : Component{ "PointLight" }
{
	setJSONAndCreate(json);
}

void PointLight::create(EditableProperties& properties) {
	mColour = properties.getOrCreate<EditableProperty::Type::colour_type>("colour");
}