//
// Created by Tim on 25/05/2017.
//

#include "AnimatedModelSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/BoneModelComponent.h"
#include "../openGLFunctions.h"
#include "../components/TransformComponent.h"
#include "../components/CameraComponent.h"
#include <glm/gtx/transform.hpp>

SYSTEM_EXPORT(AnimatedModelSystem, "animatedModelSystem")

AnimatedModelSystem::AnimatedModelSystem() 
{
	genericShader = new Shader("shaders/basic3d/3dvert.vert", "shaders/basic3d/3dfrag.frag");
	genericShader->addLoc("modelMat");
	genericShader->addLoc("viewMat");
	genericShader->addLoc("projMat");
	genericShader->addLoc("textureSampler");

	boneShader = new Shader("shaders/bone3d/boneVert.vert", "shaders/bone3d/boneFrag.frag",
									"shaders/flat.geom");
	boneShader->addLoc("modelMat");
	boneShader->addLoc("viewMat");
	boneShader->addLoc("projMat");
	boneShader->addLoc("textureSampler");
	boneShader->addLoc("boneMats");
}
AnimatedModelSystem::~AnimatedModelSystem() 
{
	delete genericShader;
	delete boneShader;
}

void AnimatedModelSystem::update(double dt) 
{
	Entity* cameraEntity = ECSManager::i()->findEntity("Camera");
	CameraComponent* camera = cameraEntity->getComponent<CameraComponent>("cameraComponent");
	
    for(auto entity : getEntities())
    {
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
        BoneModelComponent* boneModel = entity->getComponent<BoneModelComponent>("boneModelComponent");

		for(unsigned int i = 0; i < boneModel->parts.size(); i++)
		{
			Part part = boneModel->parts[i];
			glm::mat4 modelMatrix = transform->getMatrix();
			modelMatrix *= glm::translate(part.position);
			modelMatrix = glm::scale(modelMatrix, part.scale);
			modelMatrix = modelMatrix * glm::toMat4(part.rotation);

			if(boneModel->normalMeshes.find((const unsigned int) part.mesh) != boneModel->normalMeshes.end())
			{
				Mesh* mesh = boneModel->normalMeshes[part.mesh];
				genericShader->use();
				glUniformMatrix4fv(genericShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
				glUniformMatrix4fv(genericShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);

				glSetActiveTexture(GL_TEXTURE0);
				glSetBindTexture(GL_TEXTURE_2D_ARRAY, boneModel->texture.textureID);
				glUniform1i(genericShader->getLoc("textureSampler"), 0);

				glUniformMatrix4fv(genericShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

				glSetBindVertexArray(mesh->VAO);
				glDrawElements(GL_TRIANGLES, (GLint) mesh->indices.size(), GL_UNSIGNED_INT, 0);
				glSetBindVertexArray(0);
			}
			else
			{
				BoneMesh* boneMesh = boneModel->boneMeshes[part.mesh];
				boneShader->use();
				glUniformMatrix4fv(boneShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
				glUniformMatrix4fv(boneShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);

				glSetActiveTexture(GL_TEXTURE0);
				glSetBindTexture(GL_TEXTURE_2D_ARRAY, boneModel->texture.textureID);
				glUniform1i(boneShader->getLoc("textureSampler"), 0);

				glUniformMatrix4fv(boneShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

				unsigned int matsNum = (unsigned int) boneMesh->boneMats.size();
				if(matsNum)
				{
					boneMesh->transformBones((float) glfwGetTime()*24.0f);
					int matsLoc = boneShader->getLoc("boneMats");
					glUniformMatrix4fv(matsLoc, matsNum,
									   GL_FALSE, &boneMesh->boneMats.data()[0][0][0]);
				}

				glSetBindVertexArray(boneMesh->VAO);
				glDrawElements(GL_TRIANGLES, (GLint) boneMesh->indices.size(), GL_UNSIGNED_INT, 0);
				glSetBindVertexArray(0);
			}
		}
    }
}