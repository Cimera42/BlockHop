//
// Created by Jono on 17/08/2018.
//

#include "configLoader.h"

ConfigLoader::~ConfigLoader() {}

BaseAsset* ConfigLoader::loadAsset(std::string filename){
	ConfigAsset* config = new ConfigAsset(filename);
	return AssetLoader::loadAsset(filename, config);
}