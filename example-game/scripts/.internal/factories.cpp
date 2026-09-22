#include "C:\\Users\\jorda\\source\\repos\\JordanJRead\\OpenGLRenderer\\example-game\\scripts/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove(const void* json) {
	return new Move(json);
}
		
