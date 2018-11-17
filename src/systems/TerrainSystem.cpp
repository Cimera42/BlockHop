//
// Created by Tim on 16/08/2018.
//

#include "TerrainSystem.h"
#include "../ecs/ecsManager.h"
#include "../openGLFunctions.h"
#include "../components/TerrainComponent.h"
#include "../components/TransformComponent.h"
#include "../components/CameraComponent.h"

SYSTEM_EXPORT(TerrainSystem, "terrainSystem")

TerrainSystem::TerrainSystem()
{
	terrainShader = new Shader("./assets/shaders/terrain/terrain.vert", "./assets/shaders/terrain/terrain.frag");
	terrainShader->addLoc("modelMat");
	terrainShader->addLoc("viewMat");
	terrainShader->addLoc("projMat");
	terrainShader->addLoc("textureSampler");
}
TerrainSystem::~TerrainSystem()
{
	delete terrainShader;
}

void TerrainSystem::update(double dt, double alpha)
{
	Entity* cameraEntity = ECSManager::get().findEntity("Camera");
	CameraComponent* camera = cameraEntity->getComponent<CameraComponent>();

	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		auto transform = entity->getComponent<TransformComponent>();
		auto terrain = entity->getComponent<TerrainComponent>();

		glm::mat4 modelMatrix = transform->getMatrix();

		terrainShader->use();
		glUniformMatrix4fv(terrainShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(terrainShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
		glUniformMatrix4fv(terrainShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

		//Bind texture
		glSetActiveTexture(GL_TEXTURE0);
		glSetBindTexture(GL_TEXTURE_2D_ARRAY, terrain->texture);
		glUniform1i(terrainShader->getLoc("textureSampler"), 0);

		glSetBindVertexArray(terrain->VAO);
		glDrawElements(GL_TRIANGLES, (GLint) terrain->indices.size(), GL_UNSIGNED_INT, 0);
		glSetBindVertexArray(0);
	}
}