#ifndef INPUTS_JUST_PRESSED
#define INPUTS_JUST_PRESSED

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <map>

class InputsJustPressed {
public:
	void registerKey(int key);
	void registerMouseButton(int button);
	bool checkKey(int key) const;
	bool checkMouseButton(int button) const;
	void clear();

private:
	std::map<int, bool> mKeyboardInputs;
	std::map<int, bool> mMouseInputs;
};

#endif