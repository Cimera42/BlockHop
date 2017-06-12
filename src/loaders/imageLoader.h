//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_TEXTURELOADER_H
#define BLOCKHOP_TEXTURELOADER_H

#include "assetLoader.h"
#include "imageAsset.h"
#include <set>

class ImageLoader : public AssetLoader {
    //Array of all textures that are currently loaded
    std::map<std::string, ImageAsset*> fileList; //Raw image data
    //typedef std::vector<std::pair<std::vector<std::string>, GLuint> > TextureFileMap;
    //TextureFileMap textureList; //OpenGL textures
    std::map<std::set<std::string>, GLuint> textureList;


    GLuint generateTexture(std::vector<ImageAsset*> images);

public:
    ImageLoader() {
        loaderType = "texture";
    }

    /*TODO default image for asynch loading*/

    /*
     * Loads raw image data from file. This can then be used to create OpenGL
     * textures using loadTexture below.
     */
    BaseAsset* loadAsset(std::string filename);

    /*
     * Retrieves a GLuint for the texture with all the given filenames.
     * First checks for existing texture ID's and returns if so.
     * If none are found, checks fileList for each image in supplied vector.
     * Will load images that don't exist and then create a new GLuint
     */
    GLuint loadTexture(std::vector<std::string> names);
};


#endif //BLOCKHOP_TEXTURELOADER_H
