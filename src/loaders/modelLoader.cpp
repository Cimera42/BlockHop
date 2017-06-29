//
// Created by Jono on 25/06/2017.
//

#include "modelLoader.h"

ModelLoader::~ModelLoader() {}

BaseAsset* ModelLoader::loadAsset(std::string filename){
	ModelAsset* model = new ModelAsset(filename);
	return AssetLoader::loadAsset(filename, model);
}