//
// Created by Jono on 12/06/2017.
//

#include "baseAsset.h"
#include "../logger.h"

BaseAsset::BaseAsset(const std::string &filename) {
	this->filename = filename;
}

BaseAsset::~BaseAsset() {}

bool BaseAsset::load() {
	Logger()<<"Default load implementation being used to load '"<<filename<<"'. "
			"Please implement an asset class for this extension correctly.";
	return false;
}

std::string BaseAsset::getName() {
	return filename;
}