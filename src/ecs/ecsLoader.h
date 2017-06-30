//
// Created by Jono on 16/03/2017.
//

#ifndef BLOCKHOP_ECSLOADER_H
#define BLOCKHOP_ECSLOADER_H

#include "ecsManager.h"
#include <string>

class ECSLoader {
public:
	ECSLoader();
	~ECSLoader();

	//Systems or components can be created in either order
	//Probably components first
	void readStream(std::string fileName);

	//Then entities
};

#endif //BLOCKHOP_ECSLOADER_H
