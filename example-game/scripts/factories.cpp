#include "src/Move.hpp"

extern "C" __declspec(dllexport) Script* createMove() {
	return new Move;
}
		
