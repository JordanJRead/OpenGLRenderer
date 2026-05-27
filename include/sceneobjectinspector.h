#ifndef SCENE_OBJECT_INSPECTOR
#define SCENE_OBJECT_INSPECTOR

#include "viewing.h"
#include <string>
#include "componentdropdown.h"

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