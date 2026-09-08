#include "src/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove(const void* json) {
	return new Move(json);
}
		
#include "src/Move2.hpp"

extern "C" __declspec(dllexport) Script* createMove2(const void* json) {
	return new Move2(json);
}
		
