//
// Created by Tim on 12/09/2019.
//

#include "MarchingCubeRenderSystem.h"
#include "../ecs/ecsManager.h"
#include "../openGLFunctions.h"
#include "../components/CameraComponent.h"
#include "../components/MarchingCubeComponent.h"

SYSTEM_EXPORT(MarchingCubeRenderSystem, "marchingCubeRenderSystem")

MarchingCubeRenderSystem::MarchingCubeRenderSystem()
{
	marchingCubeShader = new Shader("./assets/shaders/marching/marchingCube.vert", "./assets/shaders/marching/marchingCube.frag");
	marchingCubeShader->addLoc("modelMat");
	marchingCubeShader->addLoc("viewMat");
	marchingCubeShader->addLoc("projMat");
}

MarchingCubeRenderSystem::~MarchingCubeRenderSystem()
{
	delete marchingCubeShader;
}

void MarchingCubeRenderSystem::update(double dt)
{
	auto cameraEntity = ECSManager::get().findEntity("Camera");
	auto camera = cameraEntity->getComponent<CameraComponent>();

	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		auto marchingCube = entity->getComponent<MarchingCubeComponent>();

		glm::mat4 modelMatrix = glm::mat4();

		marchingCubeShader->use();
		glUniformMatrix4fv(marchingCubeShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(marchingCubeShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
		glUniformMatrix4fv(marchingCubeShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

		glSetBindVertexArray(marchingCube->VAO);
//		glDrawElements(GL_TRIANGLES, (GLint) terrain->indices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, marchingCube->triangles.size() * 3);
		glSetBindVertexArray(0);
	}
}