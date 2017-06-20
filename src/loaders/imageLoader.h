//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_TEXTURELOADER_H
#define BLOCKHOP_TEXTURELOADER_H

#include "assetLoader.h"
#include "imageAsset.h"
#include <vector>
#include <set>

class ImageLoader : public AssetLoader {
    //Buffer of images that are currently loading
    std::vector<std::string> inLoad;
    //Array of all textures that are loaded
    std::map<std::string, ImageAsset*> fileList; //Raw image data
    //typedef std::vector<std::pair<std::vector<std::string>, GLuint> > TextureFileMap;
    //TextureFileMap textureList; //OpenGL textures
    std::map<std::set<std::string>, GLuint> textureList;

    GLuint generateTexture(std::vector<ImageAsset*> images);

public:
    ImageLoader() {
        loaderType = "image";
    }
    ~ImageLoader();

    /*TODO default image for asynch loading*/

    /*TODO, push everything back to assetLoader instead of here since everything will have
     * the same functionality and ensure we make everything private and just have the assetManager
     * as a friend class!*/

    /*
     * Loads raw image data from file. This can then be used to create OpenGL
     * textures using loadTexture below.
     */
    BaseAsset* loadAsset(std::string filename);

    /*
     * Determines if an image already exists - aka is either loading or already
     * loaded.
     */
    bool assetExists(std::string filename);

    /*
     * Finds an asset and returns it. Will wait if its in loading
     */
    BaseAsset* findAsset(std::string filename);

    /*
     * Retrieves a GLuint for the texture with all the given filenames.
     * First checks for existing texture ID's and returns if so.
     * If none are found, checks fileList for each image in supplied vector.
     * Will load images that don't exist and then create a new GLuint
     */
    GLuint loadTexture(std::vector<std::string> names);
    GLuint loadTexture(std::string name);
};


#endif //BLOCKHOP_TEXTURELOADER_H
