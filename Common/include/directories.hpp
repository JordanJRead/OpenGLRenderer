#ifndef GAME_DIRECTORY_HPP
#define GAME_DIRECTORY_HPP

#include <filesystem>
#include <string_view>

namespace Directories {
    inline std::filesystem::path gameDirectoryPath;
    inline constexpr std::string_view editorSettingsFileName{ "editorsettings.json" };
    inline constexpr std::string_view imGuiIniFileName{ "imgui.ini" };
    inline constexpr std::string_view sceneFileName{ "scene.json" };
}

#endif