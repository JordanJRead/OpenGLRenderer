#ifndef SCENE_OBJECT_INSPECTOR
#define SCENE_OBJECT_INSPECTOR

#include "componentdropdown.hpp"
#include "viewing.hpp"
#include <string>

class SceneObject;

class SceneObjectInspector {
public:
    void         toggleSelect(SceneObject* sceneObject);
    void         updateRender();
    SceneObject* getSceneObject();

private:
    Viewer<SceneObject> mSelectedObjectViewer;
    std::string         mNewComponentName;
    ComponentDropdown   mComponentDropdown;
};

#endif