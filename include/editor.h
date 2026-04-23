#ifndef EDITOR_H
#define EDITOR_H

class Scene;
class InputsJustPressed;
class Framebuffer;

class Editor {
public:
	void updateRender(Scene& scene, const InputsJustPressed& inputs, const Framebuffer& gBuffer);
	void destroyUI();
};

#endif