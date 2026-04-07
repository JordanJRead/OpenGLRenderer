#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "pointlight.h"
#include "sceneobject.h"
#include <vector>
#include "OpenGLObjects/BUF.h"
#include "shaders/shaderobject.h"
#include "shaders/shaderpointlight.h"
#include "camera.h"
#include "framebuffer.h"
#include "vertexarray.h"

class Transform;

class Scene {
public:
	Scene();
	int addObject(const std::string& objPath, const Transform& transform);
	int addPointLight(const PointLight&);
	void render(int screenWidth, int screenHeight, const Camera& camera, const ShaderObject& shader, const Framebuffer* const framebuffer);
	void renderPointLights(int screenWidth, int screenHeight, const Camera& camera, const ShaderPointLight& shader, const Framebuffer* const framebuffer);
	PointLight* getPointLight(int index) {
		if (index >= 0 && index < mPointLights.size()) {
			return &(mPointLights[index]);
		}
		return nullptr;
	}
	void updatePointLights();

private:
	std::vector<SceneObject> mObjects;
	std::vector<PointLight> mPointLights;
	BUF mPointLightBuffer;
	VertexArray mSphereVertexArray;

};

#endif