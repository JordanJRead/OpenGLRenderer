#ifndef SCENE_OBJECT_INSPECTOR
#define SCENE_OBJECT_INSPECTOR

#include "viewing.h"
#include "sceneobject.h"
#include "imgui.h"
#include "imgui/imgui_stdlib.h"

class SceneObjectInspector {
public:
	void toggleSelect(SceneObject* sceneObject) {
		mNewComponentName = "";
		if (!sceneObject) {
			mSelectedObjectViewer.lookAt(nullptr);
			return;
		}
		if (mSelectedObjectViewer.get() && mSelectedObjectViewer.get() == sceneObject) {
			mSelectedObjectViewer.lookAt(nullptr);
		}
		else {
			mSelectedObjectViewer.lookAt(&sceneObject->mViewable);
		}
	}

	void updateRender() {
		if (mSelectedObjectViewer.get()) {
			ImGui::PushID(mSelectedObjectViewer.get());
			ImGui::InputText("", &mSelectedObjectViewer.get()->getName());
			ImGui::PopID();
			mSelectedObjectViewer.get()->getTransform().renderUI();

			for (auto& component : mSelectedObjectViewer.get()->getComponents()) {
				component->renderUIProperties();
			}

			ImGui::SeparatorText("Add Commponent");
			ImGui::InputText("##NewComponentInputText", &mNewComponentName);
			ImGui::SameLine();
			if (ImGui::Button("Add")) {
				mSelectedObjectViewer.get()->addComponent(mNewComponentName); // TODO add error message on fail?
			}
			//mComponents.push_back(ComponentTypes::createFromJSON[(int)type](componentJSON));
		}

		else {
			ImGui::Text("No object selected");
		}
	}

	SceneObject* getSceneObject() { return mSelectedObjectViewer.get(); }

private:
	Viewer<SceneObject> mSelectedObjectViewer;
	std::string mNewComponentName;
};

#endif