#ifndef TEXTURE_TYPES_H
#define TEXTURE_TYPES_H

#include <array>
#include <string>
#include "texture2d.h"
#include "assimp/material.h"

namespace TextureTypes {

	enum Type {
		diffuse,
		specular,
		normal,
		max
	};

	extern std::array<std::string, (int)Type::max> names;
	extern std::array<aiTextureType, (int)Type::max> assimpTypes;
	extern std::array<Texture2D*, (int)Type::max> defaultImages;

	class MemoryGuard {
	public:
		void loadDefaultTextures();
		~MemoryGuard();
	private:
		bool mHasLoaded{ false };
	};
	extern MemoryGuard memoryGuard;
}

#endif