//
// Created by Jono on 25/06/2017.
//
#include "modelLoader.h"
#include "modelAsset.h"

ModelLoader::~ModelLoader() {}

BaseAsset* ModelLoader::loadAsset(const std::string &filename){
	ModelAsset* model = new ModelAsset(filename);
	return AssetLoader::loadAsset(filename, model);
}