#ifndef RECONSTRUCTABLE_VIEWER_H
#define RECONSTRUCTABLE_VIEWER_H

#include "nlohmann/json.hpp"
#include <memory>
#include "imgui.h"
#include "component.h"

class ReconstructableViewer {
public:
	ReconstructableViewer(const Component& component) : mCurrentMetaJSON{ component.copyInitialMetaJSON() }, mCurrentComponent{ component } {

	}

	void renderUI() const {
		// render JSON
		// ...

		if (ImGui::Button("Save")) {
			mCurrentComponent.~Component();
			Component::heapFromMetaJSON(mCurrentMetaJSON, (void*) & mCurrentComponent);
		}
	}

private:
	const Component& mCurrentComponent;
	JSON mCurrentMetaJSON;
};

#endif