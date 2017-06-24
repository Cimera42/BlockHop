#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <reactphysics3d.h>

class Logger
{
    bool endLine;
    std::string between;
	std::stringstream buffer;

public:
	Logger();
	Logger(bool doEndLine);
	Logger(std::string inBetween);
	Logger(bool doEndLine, std::string inBetween);

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
	Logger& operator<<(const glm::vec2 val);
	Logger& operator<<(const glm::vec3 val);
	Logger& operator<<(const glm::vec4 val);
	Logger& operator<<(const glm::quat val);
	Logger& operator<<(const glm::mat4 val);
	Logger& operator<<(std::ostream& (*val)(std::ostream &));
	
	Logger& operator<<(const rp3d::Vector3 val);
};

#endif // LOGGER_H_INCLUDED
