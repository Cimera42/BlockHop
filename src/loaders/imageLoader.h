//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_TEXTURELOADER_H
#define BLOCKHOP_TEXTURELOADER_H

#include "imageAsset.h"
#include "assetLoader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <set>

class ImageLoader : public AssetLoader {
	std::map<std::set<std::string>, GLuint> textureList;

	GLuint generateTexture(const std::vector<ImageAsset*> &images);
public:
	ImageLoader() {
		loaderType = "image";
	}
	~ImageLoader() override;

	BaseAsset* loadAsset(const std::string &filename) override;

	/*
	 * Retrieves a GLuint for the texture with all the given filenames.
	 * First checks for existing texture ID's and returns if so.
	 * If none are found, checks fileList for each image in supplied vector.
	 * Will load images that don't exist and then create a new GLuint
	 */
	GLuint loadTexture(const std::vector<std::string> &names);
	GLuint loadTexture(const std::string &name);
};


#endif //BLOCKHOP_TEXTURELOADER_H
