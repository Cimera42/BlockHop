//
// Created by Jono on 10/06/2017.
//

#include "imageLoader.h"

BaseAsset* ImageLoader::loadAsset(std::string filename){
    //Load image
    ImageAsset* img = new ImageAsset(filename);
    fileList.insert(std::pair<std::string, ImageAsset*>(filename, img));
}

GLuint ImageLoader::loadTexture(std::vector<std::string> names) {
    //TODO: Review if the input to this should just be a set not a vector?
    std::set<std::string> filenameSet(names.begin(), names.end());

    //Attempt to find all strings inside OpenGL textureList
    auto fileIt = textureList.find(filenameSet);
    if(fileIt != textureList.end()) {
        return fileIt->second;//Correct GLuint! Yay, dont have to do anything.
    } else {
        //For each of our file names check if we have all files loaded
        for(std::string filename : names){
            //TODO find in map and if not found, load - load synch? async? how to reset?
        }
    }
}