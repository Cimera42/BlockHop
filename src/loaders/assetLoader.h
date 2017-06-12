//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETLOADER_H
#define BLOCKHOP_ASSETLOADER_H

#include <map>
#include <string>
#include "baseAsset.h"

class AssetLoader {
protected:
    std::string loaderType;
public:
    virtual BaseAsset* loadAsset(std::string filename);
};


#endif //BLOCKHOP_ASSETLOADER_H
