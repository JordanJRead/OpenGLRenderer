#ifndef EDITOR_H
#define EDITOR_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "viewing.h"
#include <string>
#include "sceneobjectinspector.h"

class Scene;
class Inputs;
class Framebuffer;
class RenderSettings;
class App;
class SceneObject;

class Editor {
public:
	Editor();
	glm::ivec2 updateRender(const Framebuffer* const outputFramebuffer, App& app);
	void destroyUI();
	SceneObject* getSelectedObject() { return mSceneObjectInspector.getSceneObject(); }

private:
	SceneObjectInspector mSceneObjectInspector;

	void renderSceneObjectHierarchyItem(SceneObject* object, Scene& scene);
};

#endif