#ifndef EDITOR_H
#define EDITOR_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Scene;
class Inputs;
class Framebuffer;

class Editor {
public:
	void updateRender(Scene& scene, GLFWwindow* window, const Inputs& inputs, const Framebuffer& gBuffer);
	void destroyUI();
	int getSelectedObjectIndex() { return mSelectedObjectIndex; }

private:
	int mSelectedObjectIndex{ -1 };
};

#endif