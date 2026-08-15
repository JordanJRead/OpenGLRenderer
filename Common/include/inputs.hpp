#ifndef INPUTS_H
#define INPUTS_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <map>

class Inputs {
public:
	void registerKeyPress(int key);
	void registerMouseButtonPress(int button);
	void registerAllKeysDown(GLFWwindow* window);
	bool checkKeyPressed(int key) const;
	bool checkKeyDown(int key) const;
	bool checkMouseButtonPressed(int button) const;
	void clear();

private:
	std::map<int, bool> mKeyboardPresses;
	std::map<int, bool> mKeyboardDowns;
	std::map<int, bool> mMouseInputs;
};

#endif