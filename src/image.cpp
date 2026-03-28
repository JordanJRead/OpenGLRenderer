#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include "glad/glad.h"
#include <iostream>
#include <array>

Image::Image(std::string_view filePath) : mFilePath{ filePath } {
	stbi_set_flip_vertically_on_load(false);
	int channels;
	unsigned char* data = stbi_load(filePath.data(), &mWidth, &mHeight, &channels, 3);

	if (!data) {
		std::cerr << "Error loading file " << filePath << ". Reason: " << stbi_failure_reason() << "\n";
	}

	glBindTexture(GL_TEXTURE_2D, mTEX);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Image::use(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, mTEX);
}