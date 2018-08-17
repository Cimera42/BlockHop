//
// Created by Jono on 17/08/2018.
//

#ifndef BLOCKHOP_CONFIGLOADER_H
#define BLOCKHOP_CONFIGLOADER_H

#include "assetLoader.h"
#include "configAsset.h"

class ConfigLoader : public AssetLoader {

public:
	ConfigLoader() {
		loaderType = "config";
	}
	~ConfigLoader();

	BaseAsset* loadAsset(std::string filename);
};


#endif //BLOCKHOP_CONFIGLOADER_H
