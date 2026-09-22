#include "pointlight.hpp"
#include "jsonhelpers.hpp"
#include <array>
#include <stdexcept>

// TODO replace PointLight with type_name?
PointLight::PointLight(const JSON& json) : Component{ "PointLight" } {
    setJSONAndCreate(json);
}

void PointLight::create(UIProperties& properties) {
    mColour = properties.getOrCreate<UIProperty::Type::colour_type>("colour");
}