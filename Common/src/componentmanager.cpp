#include "componentmanager.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "directories.hpp"
#include "model.hpp"
#include "pointlight.hpp"
#include "script.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>

template <typename T>
std::unique_ptr<Component> createFromJSON(const JSON& json) {
    return std::make_unique<T>(json);
}

ComponentManager& ComponentManager::instance() {
    static ComponentManager manager;
    return manager;
}

struct SourceDirectoryInfo {
    size_t                                         fileCount;
    std::optional<std::filesystem::file_time_type> lastModifiedTime;
    bool operator==(const SourceDirectoryInfo&) const = default;
};

std::filesystem::file_time_type mostRecent(std::filesystem::file_time_type a,
                                           std::filesystem::file_time_type b) {
    return a.time_since_epoch() > b.time_since_epoch() ? a : b;
}

SourceDirectoryInfo getDirectoryInfo(const std::filesystem::path& directory) {
    assert(std::filesystem::is_directory(directory));

    SourceDirectoryInfo info{ 0, std::nullopt };

    for (const std::filesystem::directory_entry& entry :
         std::filesystem::directory_iterator(directory)) {

        if (entry.is_directory() && entry.path().stem().string()[0] != '.') {
            SourceDirectoryInfo subInfo{ getDirectoryInfo(entry.path()) };
            info.fileCount += subInfo.fileCount;
            if (subInfo.lastModifiedTime) {
                if (!info.lastModifiedTime)
                    info.lastModifiedTime = *subInfo.lastModifiedTime;
                else
                    info.lastModifiedTime = mostRecent(
                      *info.lastModifiedTime, *subInfo.lastModifiedTime);
            }
        } else if (entry.is_regular_file()) {
            info.fileCount++;
            auto fileRecent{ std::filesystem::last_write_time(entry.path()) };
            if (!info.lastModifiedTime)
                info.lastModifiedTime = fileRecent;
            else
                info.lastModifiedTime
                  = mostRecent(*info.lastModifiedTime, fileRecent);
        }
    }
    return info;
}

void ComponentManager::listenForScriptsUpdate(std::stop_token stop_token) {
    SourceDirectoryInfo currentInfo{ getDirectoryInfo(
      Directories::gameDirectoryPath / Directories::gameRelScriptSourcePath) };

    while (!stop_token.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SourceDirectoryInfo newInfo{ getDirectoryInfo(
          Directories::gameDirectoryPath
          / Directories::gameRelScriptSourcePath) };
        if (newInfo != currentInfo) {
            std::unique_lock<std::mutex> lock{ mFlagMutex };
            mNeedToUpdateScripts = true;
        }
        currentInfo = newInfo;
    }
}

bool ComponentManager::shouldLoadScripts(GLFWwindow* window) {
    int isFocused{ glfwGetWindowAttrib(window, GLFW_FOCUSED) };
    if (!isFocused)
        return false;

    // Check flag
    std::unique_lock<std::mutex> lock{ mFlagMutex };
    if (mNeedToUpdateScripts) {
        mNeedToUpdateScripts = false;
        return true;
    }
    return false;
}

ComponentManager::ComponentManager()
: mListenerThread{ std::bind_front(&ComponentManager::listenForScriptsUpdate,
                                   this) } {
    mStaticComponentFactories["Model"]      = createFromJSON<Model>;
    mStaticComponentFactories["PointLight"] = createFromJSON<PointLight>;
}

std::vector<std::filesystem::path>
getAllHeaderPaths(const std::filesystem::path directory) {
    std::vector<std::filesystem::path> results;
    for (const auto& item : std::filesystem::directory_iterator(directory)) {
        if (item.is_regular_file()
            && item.path().extension().string() == ".hpp") {
            results.push_back(item.path());
        } else if (item.is_directory()
                   && item.path().stem().string()[0] != '.') {
            std::vector<std::filesystem::path> subResults{ getAllHeaderPaths(
              item.path()) };
            results.insert(results.end(), subResults.begin(), subResults.end());
        }
    }
    return results;
}

void ComponentManager::loadScripts() {
    HCURSOR loadingCursor{ LoadCursor(NULL, IDC_WAIT) };
    HCURSOR prevCursor{ SetCursor(loadingCursor) };

    if (mLibraryHandle) {
        FreeLibrary(mLibraryHandle);
    }

    std::vector<std::filesystem::path> headerPaths{ getAllHeaderPaths(
      Directories::gameDirectoryPath / Directories::gameRelScriptSourcePath) };

    std::ofstream factoriesFile{ Directories::gameDirectoryPath
                                 / Directories::gameRelScriptInternalPath
                                 / "factories.cpp" };

    for (const std::filesystem::path& headerPath : headerPaths) {
        // '1 + ' is to skip the first newline of this string literal
        // (increment const char*)
        factoriesFile << 1 + R"(
#include )" << headerPath
                      << R"(

extern "C" __declspec(dllexport) Script* create)"
                      << headerPath.stem().string() << R"((const void* json) {
	return new )" << headerPath.stem().string()
                      << R"((json);
}
		)";

        factoriesFile << "\n";
    }
    factoriesFile.close();

    // CMake build
    std::filesystem::path batPath{ Directories::gameDirectoryPath
                                   / Directories::gameRelScriptInternalPath
                                   / "build.bat" };
    std::string           command = "cmd /c \"" + batPath.string() + "\"";

    std::filesystem::path programDir = std::filesystem::current_path();
    std::filesystem::current_path(batPath.parent_path());
    int result{ std::system(command.c_str()) };
    std::filesystem::current_path(programDir);

    if (result != 0) {
        std::cerr << "Error compiling scripts!\n";
        return;
    }

    // TODO debug?
    mLibraryHandle = LoadLibraryW(
      (Directories::gameDirectoryPath / Directories::gameRelScriptInternalPath
       / "build" / "Debug" / "Scripts.dll")
        .c_str());
    if (!mLibraryHandle) {
        std::cerr << "Error loading DLL!\n";
        return;
    }

    // ImGUI
    typedef void (*ImGuiContextFunc)(ImGuiContext* context);
    ImGuiContextFunc imGuiContextFunc = (ImGuiContextFunc)GetProcAddress(
      mLibraryHandle, "initializeImGuiContext");
    if (!imGuiContextFunc) {
        std::cerr << "Error loading ImGui context func!\n";
        return;
    }
    imGuiContextFunc(ImGui::GetCurrentContext());

    // Load scripts
    typedef Script* (*ScriptFactoryFunc)(const void* json);

    for (const std::filesystem::path& headerPath : headerPaths) {
        std::string funcName{ "create" };
        funcName += headerPath.stem().string();
        ScriptFactoryFunc factory
          = (ScriptFactoryFunc)GetProcAddress(mLibraryHandle, funcName.c_str());
        if (!factory) {
            std::cerr << "Error loading factory function for class "
                      << headerPath.stem() << "!\n";
            continue;
        }
        mDynamicComponentFactories[headerPath.stem().string()]
          = [factory](const JSON& json) {
                return std::unique_ptr<Component>(factory(&json));
            };
    }
    SetCursor(prevCursor);
}

std::unique_ptr<Component> ComponentManager::createComponentFromName(
  std::string_view componentTypeNameView, const JSON* const json) {
    std::string componentTypeName{ componentTypeNameView };
    // Static
    if (mStaticComponentFactories.contains(componentTypeName)) {
        auto& func = mStaticComponentFactories.at(componentTypeName);
        return func(*json);
    }

    // Dynamic
    if (mDynamicComponentFactories.contains(componentTypeName)) {
        return mDynamicComponentFactories.at(componentTypeName)(*json);
    }
    std::cerr << "ERROR: Could not load component " << componentTypeName
              << "\n";
    return nullptr;
}

// TODO move to new file?
ComponentManager::Iterator ComponentManager::begin() {
    return Iterator{ this, mStaticComponentFactories.begin(), true };
}

ComponentManager::Iterator ComponentManager::end() {
    return Iterator{ this, mDynamicComponentFactories.end(), false };
}

ComponentManager::Iterator::reference
ComponentManager::Iterator::operator*() const {
    return mMapIterator->first;
}

ComponentManager::Iterator& ComponentManager::Iterator::operator++() {
    mMapIterator++;
    if (mInStaticNames
        && mMapIterator == mOwner->mStaticComponentFactories.end()) {
        // Go into dynamic
        mMapIterator   = mOwner->mDynamicComponentFactories.begin();
        mInStaticNames = false;
    }
    return *this;
}

ComponentManager::Iterator ComponentManager::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

bool ComponentManager::Iterator::operator==(const Iterator& other) const {
    return mInStaticNames == other.mInStaticNames
           && mMapIterator == other.mMapIterator;
}

bool ComponentManager::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

ComponentManager::Iterator::Iterator(
  ComponentManager* owner,
  std::map<std::string,
           std::function<std::unique_ptr<Component>(const JSON&)>>::iterator
       iterator,
  bool inStatic)
: mOwner{ owner }, mMapIterator{ iterator }, mInStaticNames{ inStatic } {
}