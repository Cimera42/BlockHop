//
// Created by Jono on 25/06/2017.
//

#ifndef BLOCKHOP_MODELLOADER_H
#define BLOCKHOP_MODELLOADER_H

#include "assetLoader.h"
#include "modelAsset.h"

class ModelLoader : public AssetLoader {

public:
	ModelLoader() {
		loaderType = "model";
	}
	~ModelLoader();

	/*TODO default model for asynch loading*/

	BaseAsset* loadAsset(std::string filename);
};


#endif //BLOCKHOP_MODELLOADER_H
