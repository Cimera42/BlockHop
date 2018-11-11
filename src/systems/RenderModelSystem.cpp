//
// Created by Jono on 11/11/2018.
//

#include "RenderModelSystem.h"
#include "../components/AnimatedModelComponent.h"
#include "../components/TransformComponent.h"
#include "../components/CameraComponent.h"
#include "../openGLFunctions.h"
#include "../ecs/ecsManager.h"

SYSTEM_EXPORT(RenderModelSystem, "renderModelSystem")

RenderModelSystem::RenderModelSystem() {
	genericShader = new Shader("./assets/shaders/basic3d/3dvert.vert", "./assets/shaders/basic3d/3dfrag.frag");
	genericShader->addLoc("modelMat");
	genericShader->addLoc("viewMat");
	genericShader->addLoc("projMat");
	genericShader->addLoc("textureSampler");

	boneShader = new Shader("./assets/shaders/bone3d/boneVert.vert", "./assets/shaders/basic3d/3dfrag.frag",
							"./assets/shaders/flat.geom");
	boneShader->addLoc("modelMat");
	boneShader->addLoc("viewMat");
	boneShader->addLoc("projMat");
	boneShader->addLoc("textureSampler");
	boneShader->addLoc("boneMats");
}
RenderModelSystem::~RenderModelSystem() {
	delete genericShader;
	delete boneShader;
}

void RenderModelSystem::update(double dt)
{
	Entity* cameraEntity = ECSManager::get().findEntity("Camera");
	CameraComponent* camera = cameraEntity->getComponent<CameraComponent>();

	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		TransformComponent* transform = entity->getComponent<TransformComponent>();
		AnimatedModelComponent* animatedModel = entity->getComponent<AnimatedModelComponent>();

		for(auto pair : animatedModel->modelAsset->meshParts)
		{
			MeshPart *meshPart = pair.second;
			NodePart *nodePart = meshPart->nodePart;
			glm::mat4 modelMatrix = transform->getMatrix();

			//If model is animated, use animated transformation
			if(animatedModel->animated)
			{
				NodeChanging *chNode = animatedModel->FindChangingNode(nodePart->name);
				if(chNode)
				{
					modelMatrix *= chNode->collectiveMatrix;
				}
			}
			else
			{
				modelMatrix *= nodePart->defaultTransform;
			}

			//Check if mesh has bones
			bool isBoned = animatedModel->modelAsset->normalMeshes.find(meshPart->mesh) == animatedModel->modelAsset->normalMeshes.end();
			Mesh* mesh;
			Shader* shader;
			//Retrieve correct mesh and use correct shader if boned
			if(isBoned)
			{
				mesh = animatedModel->modelAsset->boneMeshes[meshPart->mesh];
				shader = boneShader;
			}
			else
			{
				mesh = animatedModel->modelAsset->normalMeshes[meshPart->mesh];
				shader = genericShader;
			}

			shader->use();
			glUniformMatrix4fv(shader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
			glUniformMatrix4fv(shader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
			glUniformMatrix4fv(shader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

			//Bind texture
			glSetActiveTexture(GL_TEXTURE0);
			glSetBindTexture(GL_TEXTURE_2D_ARRAY, animatedModel->modelAsset->texture);
			glUniform1i(shader->getLoc("textureSampler"), 0);

			//Push bone matrices to shader
			if(isBoned)
			{
				BoneMeshChanging* chBoneMesh = animatedModel->FindChangingBoneMesh(nodePart->name);
				unsigned int matsNum = (unsigned int) chBoneMesh->boneMats.size();
				if(matsNum > 0)
				{
					int matsLoc = shader->getLoc("boneMats");
					glUniformMatrix4fv(matsLoc, matsNum,
									   GL_FALSE, &chBoneMesh->boneMats.data()[0][0][0]);
				}
			}

			glSetBindVertexArray(mesh->VAO);
			glDrawElements(GL_TRIANGLES, (GLint) mesh->indices.size(), GL_UNSIGNED_INT, 0);
			glSetBindVertexArray(0);
		}
	}
}