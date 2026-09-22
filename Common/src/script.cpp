#include "script.hpp"

Script::Script(std::string_view scriptName, const JSON& json)
: Component{ scriptName, json, true } {
}