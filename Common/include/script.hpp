#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "component.hpp"
#include <string_view>
#include "nlohmann/json.hpp"

class Script : public Component {
public:
    Script(std::string_view scriptName);
};

#endif