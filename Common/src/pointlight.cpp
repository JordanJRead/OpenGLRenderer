#include "pointlight.hpp"
#include "jsonhelpers.hpp"
#include <array>
#include <stdexcept>

PointLight::PointLight(const JSON& json)
: Component{ "PointLight", json, false } {
    readOwnProperties();
}

void PointLight::readProperties(UIProperties& properties) {
    mColour = properties.getOrCreate<UIProperty::Type::colour_type>("colour");
}