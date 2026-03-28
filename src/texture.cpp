#include "texture.h"
#include "assert.h"

namespace Texture {
	std::array<std::string, (int)Type::max> names{ {
		"diffuse",
		"specular",
		"normal"
	} };

	std::array<aiTextureType, (int)Type::max> assimpTypes{ {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	} };

	std::array<Image*, (int)Type::max> defaultImages;
	MemoryGuard memoryGuard;

	void MemoryGuard::loadDefaultTextures() {
		if (!mHasLoaded) {
			mHasLoaded = true;
			int count = 0;
			defaultImages[0] = new Image{ "assets/images/white.png" }; count++;
			defaultImages[1] = new Image{ "assets/images/black.png" }; count++;
			defaultImages[2] = new Image{ "assets/images/blue.png" }; count++;
			assert(count == (int)Type::max);
		}
	}

	MemoryGuard::~MemoryGuard() {
		for (int i{ 0 }; i < (int)Type::max; ++i) {
			delete defaultImages[i];
		}
	}
}