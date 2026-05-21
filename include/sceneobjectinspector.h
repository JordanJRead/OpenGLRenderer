#ifndef SCENE_OBJECT_INSPECTOR
#define SCENE_OBJECT_INSPECTOR

#include "sceneobject.h"
#include "imgui.h"
#include <vector>
#include "reconstructableviewer.h"

class SceneObjectInspector {
public:
	void lookAt(SceneObject* sceneObject) {
		mCurrentSceneObject = sceneObject;

		if (mCurrentSceneObject) {
			for (const auto& component : mCurrentSceneObject->getComponents()) {
				mViewers.emplace_back(component.get());
			}
		}
		else {
			mViewers.clear();
		}
	}

	void renderUI() {
		if (!mCurrentSceneObject) {
			ImGui::Text("No selected object...");
			return;
		}

		for (const auto& viewer : mViewers) {
			viewer.renderUI();
		}
	}

private:
	SceneObject* mCurrentSceneObject;
	std::vector<ReconstructableViewer> mViewers;
};

#endif