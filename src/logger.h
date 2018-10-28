#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <bullet3/btBulletCollisionCommon.h>
#include <vector>
#include <map>

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

	// STD
	template <typename T>
	Logger& operator<<(std::vector<T> val);
	template <typename S, typename T>
	Logger& operator<<(std::map<S, T> val);
	Logger& operator<<(std::ostream& (*val)(std::ostream &));

	// GLM
	Logger& operator<<(glm::vec2 val);
	Logger& operator<<(glm::vec3 val);
	Logger& operator<<(glm::vec4 val);
	Logger& operator<<(glm::quat val);
	Logger& operator<<(glm::mat4 val);

	// Bullet
	Logger& operator<<(btVector3 val);
	Logger& operator<<(btQuaternion val);
	Logger& operator<<(btTransform val);
};

#endif // LOGGER_H_INCLUDED
