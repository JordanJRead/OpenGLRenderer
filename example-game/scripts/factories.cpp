#include "src/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove(const void* json) {
	return new Move(json);
}
		
#include "src/MoveDiag.hpp"

extern "C" __declspec(dllexport) Script* createMoveDiag(const void* json) {
	return new MoveDiag(json);
}
		
