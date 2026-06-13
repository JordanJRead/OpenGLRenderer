#ifndef EDITORUI_H
#define EDITORUI_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "viewing.hpp"
#include <string>
#include "sceneobjectinspector.hpp"

class Scene;
class Inputs;
class Framebuffer;
class RenderSettings;
class Editor;
class SceneObject;

class EditorUI {
public:
	EditorUI();
	glm::ivec2 updateRender(const Framebuffer* const outputFramebuffer, Editor& editor);
	void destroyUI();
	SceneObject* getSelectedObject() { return mSceneObjectInspector.getSceneObject(); }

private:
	SceneObjectInspector mSceneObjectInspector;

	void renderSceneObjectHierarchyItem(SceneObject* object, Scene& scene);
};

#endif