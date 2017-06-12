//
// Created by Tim on 25/05/2017.
//

#include "AnimatedModelSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/AnimatedModelComponent.h"
#include "../openGLFunctions.h"
#include "../components/TransformComponent.h"
#include "../components/CameraComponent.h"
#include "KeyboardInputSystem.h"
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

	KeyboardInputSystem* keyInput = ECSManager::i()->findSystem<KeyboardInputSystem>("keyboardInputSystem");
	
    for(auto entity : getEntities())
    {
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
        AnimatedModelComponent* animatedModel = entity->getComponent<AnimatedModelComponent>("animatedModelComponent");

		if(keyInput)
		{
			if(keyInput->isKeyPressed(GLFW_KEY_1))
				animatedModel->playAnimation(0);

			else if(keyInput->isKeyPressed(GLFW_KEY_2))
				animatedModel->playAnimation(1);
		}
		animatedModel->transformNodes(dt);
		
		for(unsigned int i = 0; i < animatedModel->meshParts.size(); i++)
		{
			MeshPart* meshPart = animatedModel->meshParts[i];
			NodePart* nodePart = meshPart->nodeParent;
			glm::mat4 modelMatrix = transform->getMatrix() * nodePart->collectiveMatrix;

			bool isBoned = animatedModel->normalMeshes.find((const unsigned int) meshPart->mesh) == animatedModel->normalMeshes.end();
			Mesh* mesh;
			Shader* shader;
			if(isBoned)
			{
				mesh = animatedModel->boneMeshes[meshPart->mesh];
				shader = boneShader;
			}
			else
			{
				mesh = animatedModel->normalMeshes[meshPart->mesh];
				shader = genericShader;
			}
			
			shader->use();
			glUniformMatrix4fv(shader->getLoc("viewMat"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
			glUniformMatrix4fv(shader->getLoc("projMat"), 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
			glUniformMatrix4fv(shader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

			glSetActiveTexture(GL_TEXTURE0);
			glSetBindTexture(GL_TEXTURE_2D_ARRAY, animatedModel->texture.textureID);
			glUniform1i(shader->getLoc("textureSampler"), 0);

			if(isBoned)
			{
				BoneMesh* boneMesh = static_cast<BoneMesh*>(mesh);
				unsigned int matsNum = (unsigned int) boneMesh->boneMats.size();
				if(matsNum > 0)
				{
					boneMesh->transformBones(animatedModel->nodeParts);
					int matsLoc = shader->getLoc("boneMats");
					glUniformMatrix4fv(matsLoc, matsNum,
									   GL_FALSE, &boneMesh->boneMats.data()[0][0][0]);
				}
			}

			glSetBindVertexArray(mesh->VAO);
			glDrawElements(GL_TRIANGLES, (GLint) mesh->indices.size(), GL_UNSIGNED_INT, 0);
			glSetBindVertexArray(0);
		}
    }
}