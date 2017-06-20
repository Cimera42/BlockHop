//
// Created by Jono on 10/06/2017.
//

#include "assetLoader.h"
#include "../logger.h"

BaseAsset* AssetLoader::loadAsset(std::string filename) {
    Logger(1)<<"Default assetLoader implementation being used to load '"<<filename<<"'. "
            "Please implement a loader class for this extension correctly."<<std::endl;
    return nullptr;
}

bool AssetLoader::assetExists(std::string filename) {
    return false;
}

BaseAsset* AssetLoader::findAsset(std::string filename) {
    Logger(1)<<"Default assetLoader implementation being used to find '"<<filename<<"'. "
            "Please implement a loader class for this extension correctly."<<std::endl;
    return nullptr;
}