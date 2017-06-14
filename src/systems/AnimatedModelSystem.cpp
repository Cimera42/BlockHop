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
				animatedModel->playAnimation("Armature|Grip");

			else if(keyInput->isKeyPressed(GLFW_KEY_2))
				animatedModel->playAnimation("Armature|GunFingers");

			else if(keyInput->isKeyPressed(GLFW_KEY_3))
				animatedModel->playAnimation("Armature|Chop");
		}
		//animatedModel->transformNodes((float) dt);
		
		for(auto pair : animatedModel->modelAsset->meshParts)
		{
			MeshPart* meshPart = pair.second;
			NodePart* nodePart = meshPart->nodeParent;
			NodeChanging* chNode = animatedModel->FindChangingNode(nodePart->name);
			glm::mat4 modelMatrix = transform->getMatrix() * chNode->collectiveMatrix;

			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 position;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(chNode->collectiveMatrix, scale, rotation, position, skew, perspective);
			Logger(1) << "Mesh: " << nodePart->name;
			Logger(1) << "    Position: " << position;
			Logger(1) << "    Rotation: " << rotation;
			Logger(1) << "    Scale: " << scale;


			glm::vec3 scale2;
			glm::quat rotation2;
			glm::vec3 position2;
			glm::vec3 skew2;
			glm::vec4 perspective2;
			glm::decompose(modelMatrix, scale2, rotation2, position2, skew2, perspective2);
			Logger(1) << "    Position2: " << position2;
			Logger(1) << "    Rotation2: " << rotation2;
			Logger(1) << "    Scale2: " << scale2;


			bool isBoned = animatedModel->modelAsset->normalMeshes.find(meshPart->mesh) == animatedModel->modelAsset->normalMeshes.end();
			Mesh* mesh;
			Shader* shader;
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

			glSetActiveTexture(GL_TEXTURE0);
			glSetBindTexture(GL_TEXTURE_2D_ARRAY, animatedModel->modelAsset->texture->textureID);
			glUniform1i(shader->getLoc("textureSampler"), 0);

			if(isBoned)
			{
				BoneMeshChanging* chBoneMesh = animatedModel->FindChangingBoneMesh(nodePart->name);
				BoneMesh* boneMesh = chBoneMesh->boneMesh;
				unsigned int matsNum = (unsigned int) chBoneMesh->boneMats.size();
				if(matsNum > 0)
				{
					chBoneMesh->transformBones(animatedModel->changingNodes);
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