//
// Created by Jono on 10/06/2017.
//

#include "imageLoader.h"
#include <algorithm>

BaseAsset* ImageLoader::loadAsset(std::string filename){
    //Add to the fact we're loading
    inLoad.push_back(filename);
    //Do actual loading of image
    ImageAsset* img = new ImageAsset(filename);
    //Once loading is complete add to final and remove from loading
    fileList.insert(std::pair<std::string, ImageAsset*>(filename, img));
    auto it = std::find(inLoad.begin(), inLoad.end(), filename);
    if(it != inLoad.end())
        inLoad.erase(it);
}

GLuint ImageLoader::loadTexture(std::vector<std::string> names) {
    //TODO: Review if the input to this should just be a set not a vector?
    std::set<std::string> filenameSet(names.begin(), names.end());

    //Attempt to find all strings inside OpenGL textureList
    auto fileIt = textureList.find(filenameSet);
    if(fileIt != textureList.end()) {
        return fileIt->second;//Correct GLuint! Yay, dont have to do anything.
    } else {
        //No texture already gen-ed so first:
        //For each of our file names check if we have all files loaded
        for(std::string filename : names){
            //TODO find in map and if not found, load - load synch? async? how to reset?
        }
    }
}

bool ImageLoader::assetExists(std::string filename) {
    //Check whether asset is in loading
    auto it = std::find(inLoad.begin(), inLoad.end(), filename);
    if(it != inLoad.end()) {
        return true;
    } else {
        //Check if asset is already loaded
        auto it = fileList.find(filename);
        if (it != fileList.end()) {
            return true;
        }
    }
    return false;
}