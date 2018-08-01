//
// Created by Tim on 19/07/2018.
//

#ifndef BLOCKHOP_JSON_UTILS_H
#define BLOCKHOP_JSON_UTILS_H

#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/btBulletDynamicsCommon.h>
#include <json.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nlohmann {
	template <>
	struct adl_serializer<btVector3> {
		static void from_json(const json& j, btVector3 &vec)
		{
			vec = btVector3(j[0], j[1], j[2]);
		}
	};

	template <>
	struct adl_serializer<btQuaternion> {
		static void from_json(const json& j, btQuaternion &quat)
		{
			// W is last in bullet quaternions
			quat = btQuaternion(j[1], j[2], j[3], j[0]);
		}
	};

	template <>
	struct adl_serializer<glm::vec3> {
		static void from_json(const json& j, glm::vec3 &vec)
		{
			vec = glm::vec3(j[0], j[1], j[2]);
		}
	};

	template <>
	struct adl_serializer<glm::quat> {
		static void from_json(const json& j, glm::quat &quat)
		{
			// W is first in glm quaternions
			quat = glm::quat(j[0], j[1], j[2], j[3]);
		}
	};
}

#endif //BLOCKHOP_JSON_UTILS_H
