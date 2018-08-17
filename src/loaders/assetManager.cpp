//
// Created by Jono on 10/06/2017.
//

#include "assetManager.h"
#include <fstream>

AssetManager *AssetManager::c_instance = 0;

AssetManager::AssetManager() {}

void AssetManager::readConfig(){

	//TODO possibly replace this with non-hard? Is it worth it? - No its not, just remember to do it
	exportedLoaders.insert(std::pair<std::string, AssetLoader*>("config", new ConfigLoader()));
	exportedLoaders.insert(std::pair<std::string, AssetLoader*>("image", new ImageLoader()));
	exportedLoaders.insert(std::pair<std::string, AssetLoader*>("model", new ModelLoader()));

	std::string config = "./conf/assetLoader.conf";
	std::ifstream i(config);
	if(!i.is_open()) {
		Logger() << "AssetLoader failed to start as the file '" + config + "' could not be opened." << std::endl;
		exit(-4);
	}

	//Attempt to read file into json object
	json j;
	try {
		i >> j;

		//Use json file to prime the asset loader
		for(json loaderType : j) {
			std::string type = loaderType["loaderType"];
			auto it = exportedLoaders.find(type);
			for(json extension : loaderType["extensions"]) {
				std::string ext = extension.get<std::string>();
				if(it == exportedLoaders.end()) {
					Logger() << "Loader for '" << ext
							  << "' doesn't exist. Assets of this type will not work correctly"<<std::endl;
				}
				else {
					extLoaders.insert(std::pair<std::string, AssetLoader*>(ext, it->second));
				}
			}
			if(it != exportedLoaders.end()) {
				it->second->defaultFilename = loaderType["default"]["filename"];
			}
		}
	}catch (std::invalid_argument invalidArgument) {
		Logger() << invalidArgument.what();
		Logger()<<"Cannot read config file, "+config+". AssetLoader will not function correctly."<<std::endl;
		exit(-6);
	}catch (...) {
		Logger()<<"Cannot read config file, "+config+". AssetLoader will not function correctly."<<std::endl;
		exit(-6);
	}

	i.close();
}

void AssetManager::loadDefault() {
	// Load all default assets after we have primed our loaders and setup the engine
	for( auto it = exportedLoaders.begin(); it != exportedLoaders.end(); it++) {
		it->second->defaultAsset = loadSync(it->second->defaultFilename);
	}
}

AssetManager::~AssetManager() {
	for (auto it : exportedLoaders) {
		delete it.second;
	}
}

BaseAsset* AssetManager::loadSync(std::string filename) {
	//Determine file extension
	auto dotPos = filename.find_last_of(".");
	std::string ext = filename.substr(dotPos+1);

	//Attempt to find file extension in map to get specific loader
	auto extLoader = extLoaders.find(ext);

	//Load using loader
	if(extLoader != extLoaders.end()) {
		//Attempt to find and return, else load it
		BaseAsset* asset = extLoader->second->findAsset(filename);
		if(asset == nullptr) {
			return extLoader->second->loadAsset(filename);
		}
		return asset;
	} else {
		Logger()<<"Could not find a loader for the file '"<<filename<<"'."<<std::endl;
	}
	return nullptr;
}

AssetLoader* AssetManager::getLoader(std::string loaderName) {
	//Find within exportedLoaders
	auto it = exportedLoaders.find(loaderName);

	if(it != exportedLoaders.end()) {
		return it->second;
	} else {
		Logger()<<"Cannot find loader with given name '"<<loaderName<<"', ensure loader exists"
				"inside asset configuration file."<<std::endl;
	}
	return nullptr;
}