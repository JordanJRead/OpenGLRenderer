#include "sceneobjectinspector.h"
#include "sceneobject.h"
#include "imgui.h"
#include "imgui/imgui_stdlib.h"

void SceneObjectInspector::toggleSelect(SceneObject* sceneObject) {
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

void SceneObjectInspector::updateRender() {
	if (mSelectedObjectViewer.get()) {
		ImGui::PushID(mSelectedObjectViewer.get());
		ImGui::InputText("", &mSelectedObjectViewer.get()->getName());
		ImGui::PopID();
		mSelectedObjectViewer.get()->getTransform().renderUI();

		auto& components{ mSelectedObjectViewer.get()->getComponents() };
		for (auto componentIt{ components.begin() }; componentIt != components.end(); ) {
			(*componentIt)->renderUIProperties();
			ImGui::PushID(&(*componentIt));
			if (ImGui::Button("X")) {
				componentIt = components.erase(componentIt);
			}
			else {
				componentIt++;
			}
			ImGui::PopID();
		}

		ImGui::SeparatorText("Add Commponent");
		ImGui::InputText("##NewComponentInputText", &mNewComponentName);
		ImGui::SameLine();
		if (ImGui::Button("Add")) {
			mSelectedObjectViewer.get()->addComponent(mNewComponentName); // TODO add error message on fail?
		}
	}

	else {
		ImGui::Text("No object selected");
	}
}

SceneObject* SceneObjectInspector::getSceneObject() { return mSelectedObjectViewer.get(); }
