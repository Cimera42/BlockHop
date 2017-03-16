#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <sstream>
#include <chrono>
#include <string>

struct Logger
{
    bool endLine;
    std::string between;

    Logger()
    {
        endLine = false;
        between = "";
    }
    Logger(bool doEndLine)
    {
        endLine = doEndLine;
    }
    Logger(std::string inBetween)
    {
        between = inBetween;
    }
    Logger(bool doEndLine, std::string inBetween)
    {
        endLine = doEndLine;
        between = inBetween;
    }

    std::stringstream buffer;
    //Function for general types
    template <typename T>
    Logger& operator<<(const T& val)
    {
        //Push input to stringstream
        buffer << val << between;
        return *this;
    }

    //Function for special stream types, eg endl
    Logger& operator<<(std::ostream& (*val)(std::ostream &))
    {
        buffer << val << between;
        return *this;
    }

    //Print entire string buffer at end of << chain
    //when logger object is destroyed
    ~Logger()
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
};

#endif // LOGGER_H_INCLUDED
