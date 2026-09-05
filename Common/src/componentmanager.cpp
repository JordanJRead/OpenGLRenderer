#include "componentmanager.hpp"

#include "model.hpp"
#include "pointlight.hpp"
#include "directories.hpp"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include "script.hpp"

template <typename T>
std::unique_ptr<Component> createFromJSON(const JSON& json) {
	return std::make_unique<T>(json);
}

ComponentManager& ComponentManager::instance() {
	static ComponentManager manager;
	return manager;
}

ComponentManager::ComponentManager() {
	mStaticComponentFactories["Model"] = createFromJSON<Model>;
	mStaticComponentFactories["PointLight"] = createFromJSON<PointLight>;
}

void ComponentManager::loadScripts() {
	if (mLibraryHandle) {
		FreeLibrary(mLibraryHandle);
	}

	std::vector<std::string> classNames;
	for (const auto& item : std::filesystem::directory_iterator(Directories::gameDirectoryPath / "scripts" / "src")) {
		if (item.is_regular_file() && item.path().extension().string() == ".cpp") {
			classNames.push_back(item.path().stem().string());
		}
	}

	std::ofstream factoriesFile{ Directories::gameDirectoryPath / "scripts" / "factories.cpp" };

	for (const std::string& className : classNames) {
		factoriesFile << 1 + R"(
#include "src/)" << className << R"(.hpp"

extern "C" __declspec(dllexport) Script* create)" << className << R"((const void* json) {
	return new )" << className << R"((json);
}
		)";

		factoriesFile << "\n";
	}
	factoriesFile.close();

	std::filesystem::path batPath{ Directories::gameDirectoryPath / "scripts" / "build.bat" };
	std::string command = "cmd /c \"" + batPath.string() + "\"";

	std::filesystem::path programDir = std::filesystem::current_path();
	std::filesystem::current_path(batPath.parent_path());
	int result{ std::system(command.c_str()) };
	std::filesystem::current_path(programDir);

	if (result != 0) {
		std::cerr << "Error compiling scripts!\n";
		return;
	}
	// TODO debug?
	// TODO re-create all scripts after a reload
	mLibraryHandle = LoadLibraryW((Directories::gameDirectoryPath / "scripts" / "build" / "Debug" / "Scripts.dll").c_str());
	if (!mLibraryHandle) {
		std::cerr << "Error loading DLL!\n";
		return;
	}

	// ImGUI
	typedef void (*ImGuiContextFunc)(ImGuiContext* context);
	ImGuiContextFunc imGuiContextFunc = (ImGuiContextFunc)GetProcAddress(mLibraryHandle, "initializeImGuiContext");
	if (!imGuiContextFunc) {
		std::cerr << "Error loading ImGui context func!\n";
		return;
	}
	imGuiContextFunc(ImGui::GetCurrentContext());

	// Load scripts
	typedef Script* (*ScriptFactoryFunc)(const void* json);

	for (const std::string& className : classNames) {
		std::string funcName{ "create" };
		funcName += className;
		ScriptFactoryFunc factory = (ScriptFactoryFunc)GetProcAddress(mLibraryHandle, funcName.c_str());
		if (!factory) {
			std::cerr << "Error loading factory function for class " << className << "!\n";
			return;
		}
		mDynamicComponentFactories[className] = [factory](const JSON& json){
			return std::unique_ptr<Component>(factory(&json));
		};
	}
}

std::unique_ptr<Component> ComponentManager::createComponentFromName(std::string_view componentTypeNameView, const JSON* const json) {
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

ComponentManager::Iterator::reference ComponentManager::Iterator::operator*() const {
    return mMapIterator->first;
}

ComponentManager::Iterator& ComponentManager::Iterator::operator++() {
	mMapIterator++;
    if (mInStaticNames && mMapIterator == mOwner->mStaticComponentFactories.end()) {
        // Go into dynamic
        mMapIterator = mOwner->mDynamicComponentFactories.begin();
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
    return mInStaticNames == other.mInStaticNames && mMapIterator == other.mMapIterator;
}

bool ComponentManager::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

ComponentManager::Iterator::Iterator(ComponentManager* owner, std::map<std::string, std::function<std::unique_ptr<Component>(const JSON&)>>::iterator iterator, bool inStatic)
    : mOwner{ owner }
    , mMapIterator{ iterator }
    , mInStaticNames{ inStatic }
{
}