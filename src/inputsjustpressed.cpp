#include "inputsjustpressed.h"
#include "app.h"

void InputsJustPressed::registerKey(int key) {
	mKeyboardInputs.insert({ key, true });
}

void InputsJustPressed::registerMouseButton(int button) {
	mMouseInputs.insert({ button, true });
}

bool InputsJustPressed::checkKey(int key) const {
	return mKeyboardInputs.contains(key);
}

bool InputsJustPressed::checkMouseButton(int button) const {
	return mMouseInputs.contains(button);
}

void InputsJustPressed::clear() {
	mKeyboardInputs.clear();
	mMouseInputs.clear();
}
