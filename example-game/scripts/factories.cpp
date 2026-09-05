#include "src/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove(const void* json) {
	return new Move(json);
}
		
