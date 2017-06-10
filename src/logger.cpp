//
// Created by Tim on 8/06/2017.
//

#include "logger.h"
#include <string>

Logger::Logger()
{
	endLine = false;
	between = "";
}
Logger::Logger(bool doEndLine)
{
	endLine = doEndLine;
	between = "";
}
Logger::Logger(std::string inBetween)
{
	endLine = false;
	between = inBetween;
}
Logger::Logger(bool doEndLine, std::string inBetween)
{
	endLine = doEndLine;
	between = inBetween;
}

Logger::~Logger()
{
	if(endLine)
	{
		time_t current_time;
		struct tm * time_info;
		std::time(&current_time);
		char timeString[9];  // space for "HH:MM:SS\0"
		time_info = localtime(&current_time);
		strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
		std::cout << "[" << timeString << "] ";

	}

	std::cout << buffer.str();
	if(endLine)
		std::cout << std::endl;
}

Logger& Logger::operator<<(const glm::vec3 val)
{
	//Push input to stringstream
	buffer << val.x << ", " << val.y << ", " << val.z << between;
	return *this;
}

Logger& Logger::operator<<(const glm::vec4 val)
{
	//Push input to stringstream
	buffer << val.w << ", " << val.x << ", " << val.y << ", " << val.z << between;
	return *this;
}

Logger& Logger::operator<<(const glm::quat val)
{
	(*this) << glm::vec4(val.w,val.x,val.y,val.z);
	return *this;
}

Logger& Logger::operator<<(const glm::mat4 val)
{
	//Push input to stringstream
	(*this) << val[0] << val[1] << val[2] << val[3];
	return *this;
}

//Function for special stream types, eg endl
Logger& Logger::operator<<(std::ostream& (*val)(std::ostream &))
{
	buffer << val << between;
	return *this;
}