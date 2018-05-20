//
// Created by Jono on 10/06/2017.
//
#include "imageLoader.h"
#include "../openGLFunctions.h"
#include "../logger.h"

ImageLoader::~ImageLoader() {
	//Delete textures
	for(auto tex : textureList) {
		glDeleteTextures(1, &tex.second);
	}
	textureList.clear();
}

BaseAsset* ImageLoader::loadAsset(const std::string &filename) {
	ImageAsset* model = new ImageAsset(filename);
	return AssetLoader::loadAsset(filename, model);
}

GLuint ImageLoader::loadTexture(const std::vector<std::string> &names) {
	std::set<std::string> filenameSet(names.begin(), names.end());

	//Attempt to find all strings inside OpenGL textureList
	auto fileIt = textureList.find(filenameSet);
	if(fileIt != textureList.end()) {
		return fileIt->second;//Correct GLuint! Yay, dont have to do anything.
	} else {
		//No texture already gen-ed so first:
		//For each of our file names check if we have all files loaded
		std::vector<ImageAsset*> images;
		for(std::string filename : names){
			//Find in map and if not found, load - load sync
			//(basically ensure preloaded to avoid this!)
			ImageAsset* img = static_cast<ImageAsset*>(findAsset(filename));
			if(img == nullptr) {
				img = static_cast<ImageAsset*>(loadAsset(filename));
			}
			images.push_back(img);
		}
		//Gen our textures
		//TODO better error handling - use default if not loaded correctly etc.
		GLuint newTexture = generateTexture(images);
		//Add to map for later use
		textureList.insert(std::pair<std::set<std::string>, GLuint>(filenameSet, newTexture));
		return newTexture;
	}
}

GLuint ImageLoader::loadTexture(const std::string &name) {
	std::vector<std::string> nameVec;
	nameVec.push_back(name);
	return loadTexture(nameVec);
}

GLuint ImageLoader::generateTexture(const std::vector<ImageAsset*> &images) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	//Get max sizings
	int maxW = 0;
	int maxH = 0;
	for(ImageAsset* img : images) {
		if(img->height > maxH)
			maxH = img->height;
		if(img->width > maxW)
			maxW = img->width;
	}

	//Setup
	glSetBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, images.size(), GL_RGBA8, maxW,maxH, images.size());

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//Use each image data to generate textures
	bool isLoaded = true;
	for(unsigned int i = 0; i < images.size(); i++)
	{
		if(images[i]->imageData)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, maxW, maxH, 1, GL_RGBA, GL_UNSIGNED_BYTE,
							images[i]->imageData);
			GLenum err = glGetError();
			if (!err) {
				Logger() << "Texture loaded: " << images[i]->getName();
			} else {
				Logger() << "Texture load failed: " << images[i]->getName() << " - " << std::hex << err;
				isLoaded = false;
				break;
			}
		}
	}

	//Delete if not loaded correctly
	if(!isLoaded) {
		glDeleteTextures(1, &textureID);
	}

	return textureID;
}