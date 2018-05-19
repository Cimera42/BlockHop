//
// Created by Jono on 10/06/2017.
//

#include "assetLoader.h"
#include "../logger.h"
#include <algorithm>
#include <chrono>
#include <thread>

AssetLoader::AssetLoader() {}
AssetLoader::~AssetLoader() {
	//Wait for everything in load to finalise and then remove
	while(inLoad.begin() != inLoad.end()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //Sleep and recheck load
	}

	for(auto img : fileList) {
		delete img.second;
	}
	fileList.clear();
}

BaseAsset* AssetLoader::getDefaultAsset() const {
	return defaultAsset;
}

BaseAsset* AssetLoader::loadAsset(std::string filename, BaseAsset* newAsset){
	//Add to the fact we're loading
	inLoad.push_back(filename);
	//Do actual loading
	if(!newAsset->load()) {
		//Delete the new asset and add the default for this
		delete newAsset;
		newAsset = defaultAsset;
	}
	//Once loading is complete add to final and remove from loading
	fileList.insert(std::pair<std::string, BaseAsset*>(filename, newAsset));
	auto it = std::find(inLoad.begin(), inLoad.end(), filename);
	if(it != inLoad.end())
		inLoad.erase(it);
	return newAsset;
}

BaseAsset* AssetLoader::loadAsset(std::string filename) {
	Logger()<<"Default asset type attempting to be used for loading '"<<filename<<"'. "
			"Please implement a loader with loadAsset and a correct asset type to use correctly.";
}

bool AssetLoader::assetExists(std::string filename) {
	//Check whether asset is in loading
	auto it = std::find(inLoad.begin(), inLoad.end(), filename);
	if(it != inLoad.end()) {
		return true;
	} else {
		//Check if asset is already loaded
		auto assetIt = fileList.find(filename);
		if (assetIt != fileList.end()) {
			return true;
		}
	}
	return false;
}

BaseAsset* AssetLoader::findAsset(std::string filename) {
	//If in loading, wait until loaded
	while(std::find(inLoad.begin(), inLoad.end(), filename) != inLoad.end()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //Sleep and recheck load
	}
	//If asset is now loaded, return
	auto it = fileList.find(filename);
	if (it != fileList.end()) {
		return it->second;
	}
	//Else doesnt exist
	return nullptr;
}