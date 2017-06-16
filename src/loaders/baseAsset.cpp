//
// Created by Jono on 12/06/2017.
//

#include "baseAsset.h"
#include "../logger.h"

BaseAsset::BaseAsset(std::string filename) {
    this->filename = filename;
}

BaseAsset::~BaseAsset() {}

void BaseAsset::load(std::string filename) {
    Logger(1)<<"Default load implementation being used to load '"<<filename<<"'. "
            "Please implement an asset class for this extension correctly."<<std::endl;
}

std::string BaseAsset::getName() {
    return filename;
}