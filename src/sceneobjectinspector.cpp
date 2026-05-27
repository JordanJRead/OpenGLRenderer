#include "sceneobjectinspector.h"
#include "sceneobject.h"
#include "imgui.h"
#include "imgui/imgui_stdlib.h"

void SceneObjectInspector::toggleSelect(SceneObject* sceneObject) {
	mNewComponentName = "";
	mComponentDropdown.reset();
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

		ImGui::SeparatorText("Transform");
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
		mComponentDropdown.renderUI();
		ImGui::SameLine();
		if (ImGui::Button("Add")) {
			mSelectedObjectViewer.get()->addComponent(mComponentDropdown.getCurrentType());
		}
	}

	else {
		ImGui::Text("No object selected");
	}
}

SceneObject* SceneObjectInspector::getSceneObject() { return mSelectedObjectViewer.get(); }
