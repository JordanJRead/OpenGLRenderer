#ifndef EDITOR_H
#define EDITOR_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Scene;
class InputsJustPressed;
class Framebuffer;

class Editor {
public:
	void updateRender(Scene& scene, GLFWwindow* window, const InputsJustPressed& inputs, const Framebuffer& gBuffer);
	void destroyUI();

private:
	int mSelectedObjectIndex{ -1 };
};

#endif