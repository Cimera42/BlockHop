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

bool BaseAsset::save() {
	Logger()<<"Default save implementation being used to save '"<<filename<<"'. "
			"Please implement an asset class for this extension correctly."<<std::endl;
	return false;
}

std::string BaseAsset::getName() {
	return filename;
}