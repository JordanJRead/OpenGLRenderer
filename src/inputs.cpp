#include "inputs.h"
#include "app.h"

void Inputs::registerKeyPress(int key) {
	mKeyboardPresses.insert({ key, true });
}

void Inputs::registerMouseButtonPress(int button) {
	mMouseInputs.insert({ button, true });
}

void Inputs::registerAllKeysDown(GLFWwindow* window) {
	for (int i{ 0 }; i <= GLFW_KEY_LAST; ++i) {
		if (glfwGetKey(window, i)) {
			mKeyboardDowns.insert({ i, true });
		}
	}
}

bool Inputs::checkKeyPressed(int key) const {
	return mKeyboardPresses.contains(key);
}

bool Inputs::checkMouseButtonPressed(int button) const {
	return mMouseInputs.contains(button);
}

bool Inputs::checkKeyDown(int key) const {
	return mKeyboardDowns.contains(key);
}

void Inputs::clear() {
	mKeyboardPresses.clear();
	mKeyboardDowns.clear();
	mMouseInputs.clear();
}
