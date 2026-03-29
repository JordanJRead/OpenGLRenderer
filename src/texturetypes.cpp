#include "texturetypes.h"
#include "assert.h"
#include "texture2d.h"

namespace TextureTypes {
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

	std::array<Texture2D*, (int)Type::max> defaultImages;
	MemoryGuard memoryGuard;

	void MemoryGuard::loadDefaultTextures() {
		if (!mHasLoaded) {
			mHasLoaded = true;
			int count = 0;
			defaultImages[0] = new Texture2D{ "assets/images/white.png", false }; count++;
			defaultImages[1] = new Texture2D{ "assets/images/black.png", false }; count++;
			defaultImages[2] = new Texture2D{ "assets/images/blue.png",  false }; count++;
			assert(count == (int)Type::max);
		}
	}

	MemoryGuard::~MemoryGuard() {
		for (int i{ 0 }; i < (int)Type::max; ++i) {
			delete defaultImages[i];
		}
	}
}