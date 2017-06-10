//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_TEXTURELOADER_H
#define BLOCKHOP_TEXTURELOADER_H

#include "assetLoader.h"

class TextureLoader : public AssetLoader {
public:
    TextureLoader() {
        loaderType = "texture";
    }
};


#endif //BLOCKHOP_TEXTURELOADER_H
