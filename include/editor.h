#ifndef EDITOR_H
#define EDITOR_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

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
	SceneObject* getSelectedObject() { return mSelectedObject; }

private:
	SceneObject* mSelectedObject{ nullptr };
	void renderSceneObject(SceneObject* object, Scene& scene);
};

#endif