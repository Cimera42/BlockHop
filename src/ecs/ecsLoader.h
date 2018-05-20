//
// Created by Jono on 16/03/2017.
//

#ifndef BLOCKHOP_ECSLOADER_H
#define BLOCKHOP_ECSLOADER_H

#include <string>

class ECSLoader {
public:
	ECSLoader() = default;
	~ECSLoader() = default;

	//Systems or components can be created in either order
	//Probably components first
	void readStream(const std::string &fileName);

	//Then entities
};

#endif //BLOCKHOP_ECSLOADER_H
