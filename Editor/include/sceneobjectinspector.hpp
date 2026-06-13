#ifndef SCENE_OBJECT_INSPECTOR
#define SCENE_OBJECT_INSPECTOR

#include "viewing.hpp"
#include <string>
#include "componentdropdown.hpp"

class SceneObject;

class SceneObjectInspector {
public:
	void toggleSelect(SceneObject* sceneObject);
	void updateRender();
	SceneObject* getSceneObject();

private:
	Viewer<SceneObject> mSelectedObjectViewer;
	std::string mNewComponentName;
	ComponentDropdown mComponentDropdown;
};

#endif