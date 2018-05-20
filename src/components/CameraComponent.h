//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_CAMERACOMPONENT_H
#define BLOCKHOP_CAMERACOMPONENT_H

#include "../ecs/component.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <json.hpp>

using json = nlohmann::json;

class CameraComponent : public Component
{
	static bool exported;

	float fov;
	float aspectRatio;
	float nearDist;
	float farDist;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:
	void setValues(const json &inValues) override;

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up);
	void perspective();

	friend std::ostream &operator<< (std::ostream &os, CameraComponent const &c) {
		os << "CameraComponent";
		return os;
	}
};

#endif //BLOCKHOP_CAMERACOMPONENT_H