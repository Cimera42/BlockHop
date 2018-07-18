#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <reactphysics3d.h>
#include <bullet3/btBulletCollisionCommon.h>

class Logger
{
	bool endLine;
	std::string between;
	std::stringstream buffer;

public:
	Logger();
	explicit Logger(bool doEndLine);
	explicit Logger(const std::string& inBetween);
	Logger(bool doEndLine, const std::string& inBetween);

	//Print entire string buffer at end of << chain
	//when logger object is destroyed
	~Logger();

	//Function for general types
	template <typename T>
	Logger& operator<<(const T& val)
	{
		//Push input to stringstream
		buffer << val << between;
		return *this;
	}
	Logger& operator<<(glm::vec2 val);
	Logger& operator<<(glm::vec3 val);
	Logger& operator<<(glm::vec4 val);
	Logger& operator<<(glm::quat val);
	Logger& operator<<(glm::mat4 val);
	Logger& operator<<(std::ostream& (*val)(std::ostream &));

	Logger& operator<<(rp3d::Vector3 val);

	Logger& operator<<(btVector3 val);
	Logger& operator<<(btQuaternion val);
};

#endif // LOGGER_H_INCLUDED
