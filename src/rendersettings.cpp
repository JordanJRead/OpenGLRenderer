#include "rendersettings.h"
#include <array>
#include <iostream>

RenderSettings::RenderSettings() {
	mBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mBuffer);
	sendToGPU();
}

void RenderSettings::updateGPU() {
	bool same = mPrevHighlightColour == mHighlightColour && mPrevPointLightRenderScale == mPointLightRenderScale;
	if (!same) {
		mPrevHighlightColour = mHighlightColour;
		mPrevPointLightRenderScale = mPointLightRenderScale;
		sendToGPU();
	}
}

void RenderSettings::sendToGPU() {
	std::cout << "sent\n";
	mBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	std::array<float, 4> data;
	data[0] = mHighlightColour.r;
	data[1] = mHighlightColour.g;
	data[2] = mHighlightColour.b;
	data[3] = mPointLightRenderScale;
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(float), this, GL_STATIC_DRAW);
}