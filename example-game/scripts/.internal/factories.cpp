#include "C:\\Users\\jorda\\source\\repos\\JordanJRead\\OpenGLRenderer\\example-game\\scripts/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove(const void* json) {
	return new Move(json);
}
		
#include "C:\\Users\\jorda\\source\\repos\\JordanJRead\\OpenGLRenderer\\example-game\\scripts/Move3.hpp"

extern "C" __declspec(dllexport) Script* createMove3(const void* json) {
	return new Move3(json);
}
		
