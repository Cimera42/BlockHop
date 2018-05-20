//
// Created by Tim on 20/05/2018.
//
#include "utils.h"
#include <sstream>

std::vector<std::string> splitString(const std::string &s, char delimiter)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (getline(ss, item, delimiter)) {
		tokens.push_back(item);
	}
	return tokens;
}