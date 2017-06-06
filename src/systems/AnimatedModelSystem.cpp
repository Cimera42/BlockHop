//
// Created by Tim on 25/05/2017.
//

#include "AnimatedModelSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/AnimatedModelComponent.h"
#include "../openGLFunctions.h"
#include "../components/TransformComponent.h"
#include "../components/CameraComponent.h"
#include <glm/gtx/transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

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
        AnimatedModelComponent* animatedModel = entity->getComponent<AnimatedModelComponent>("animatedModelComponent");

		for(unsigned int i = 0; i < animatedModel->meshParts.size(); i++)
		{
			MeshPart* meshPart = animatedModel->meshParts[i];
			NodePart* nodePart = meshPart->nodeParent;
			glm::mat4 modelMatrix = transform->getMatrix() * nodePart->collectiveMatrix;
					  
			if(animatedModel->normalMeshes.find((const unsigned int) meshPart->mesh) != animatedModel->normalMeshes.end())
			{
				Mesh* mesh = animatedModel->normalMeshes[meshPart->mesh];
				genericShader->use();
				glUniformMatrix4fv(genericShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
				glUniformMatrix4fv(genericShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);

				glSetActiveTexture(GL_TEXTURE0);
				glSetBindTexture(GL_TEXTURE_2D_ARRAY, animatedModel->texture.textureID);
				glUniform1i(genericShader->getLoc("textureSampler"), 0);

				glUniformMatrix4fv(genericShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

				glSetBindVertexArray(mesh->VAO);
				glDrawElements(GL_TRIANGLES, (GLint) mesh->indices.size(), GL_UNSIGNED_INT, 0);
				glSetBindVertexArray(0);
			}
			else
			{
				BoneMesh* boneMesh = animatedModel->boneMeshes[meshPart->mesh];
				boneShader->use();
				std::for_each(boneMesh->boneMats.begin(), boneMesh->boneMats.end(), [](const glm::mat4 mat){
					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 position;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(mat, scale, rotation, position, skew, perspective);
					Logger(1) << "    Position: " << position;
					Logger(1) << "    Rotation: " << rotation;
					Logger(1) << "    Scale: " << scale;
				});
				glUniformMatrix4fv(boneShader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
				glUniformMatrix4fv(boneShader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);

				glSetActiveTexture(GL_TEXTURE0);
				glSetBindTexture(GL_TEXTURE_2D_ARRAY, animatedModel->texture.textureID);
				glUniform1i(boneShader->getLoc("textureSampler"), 0);

				glUniformMatrix4fv(boneShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

				unsigned int matsNum = (unsigned int) boneMesh->boneMats.size();
				if(matsNum)
				{
					boneMesh->transformBones(animatedModel->nodeParts);
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