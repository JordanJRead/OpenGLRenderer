#ifndef GAME_DIRECTORY_HPP
#define GAME_DIRECTORY_HPP

#include <filesystem>
#include <string_view>

inline std::filesystem::path gGameDirectoryPath;
inline constexpr std::string_view gEditorSettingsFileName{ "editorsettings.json" };
inline constexpr std::string_view gImGuiIniFileName{ "imgui.ini" };
inline constexpr std::string_view gSceneFileName{ "scene.json" };

#endif