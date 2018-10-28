//
// Created by Jono on 12/06/2017.
//

#include "baseAsset.h"
#include "../logger.h"

BaseAsset::BaseAsset(std::string filename) {
	this->filename = filename;
}

BaseAsset::~BaseAsset() {}

bool BaseAsset::load() {
	Logger()<<"Default load implementation being used to load '"<<filename<<"'. "
			"Please implement an asset class for this extension correctly."<<std::endl;
	return false;
}

// TODO: Determine if we need saving to go through the same AssetManager -> AssetLoader -> Asset channels
// or if we can just save dirrectly (since we likely have a pointer to the asset instance)
bool BaseAsset::save() {
	Logger()<<"Default save implementation being used to save '"<<filename<<"'. "
			"Please implement an asset class for this extension correctly."<<std::endl;
	return false;
}

std::string BaseAsset::getName() {
	return filename;
}

void BaseAsset::setMutable(bool v) {
	isMutable = v;
}