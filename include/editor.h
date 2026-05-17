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

class Editor {
public:
	Editor();
	glm::ivec2 updateRender(const Framebuffer* const outputFramebuffer, App& app);
	void destroyUI();
	int getSelectedObjectIndex() { return mSelectedObjectIndex; }

private:
	int mSelectedObjectIndex{ -1 };
};

#endif