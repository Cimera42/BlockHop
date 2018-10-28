//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETMANAGER_H
#define BLOCKHOP_ASSETMANAGER_H


#include <string>
#include <json.hpp>
#include "assetLoader.h"
#include "configLoader.h"
#include "imageLoader.h"
#include "modelLoader.h"
#include "../logger.h"
using json = nlohmann::json;

class AssetManager {
public:
	/* Singleton */
	static AssetManager& get()
	{
		static AssetManager c_instance;
		return c_instance;
	}

private:
	AssetManager();
	/*
	 * Base asset loading class - singleton & factory pattern
	 * Maintains a map of loaders which contain all assets that have been loaded into the engine
	 *
	 * Two flavours of loading:
	 * - Synchronous - (pre-loading)
	 * - Asynchronous - (fly-loading)
	 *
	 * On engine startup, the asset manager will load a json file for configuration
	 * This config will contain:
	 *  - The extensions which are available for each asset (eg. jpg, png)
	 *  - The specific loader each extension uses (eg. textureLoader)
	 *  - A default for each loader to be used during asynchronous loading
	 */
	std::map<std::string, AssetLoader*> extLoaders; //Loader for each extension

	std::map<std::string, AssetLoader*> exportedLoaders; //Loader with name

public:
	AssetManager(AssetManager const&) = delete;
	void operator=(AssetManager const&) = delete;
	~AssetManager();

	void readConfig();
	/*
	 * Load default assets after reading config, at a convenient time
	 */
	void loadDefault();


	/*
	 * Load a raw asset from file. Will automatically determine the loader to use for the
	 * filename/extension given and will then check if the asset is already loaded. If not
	 * it will conduct a load synchronously.
	 */
	BaseAsset* loadSync(std::string filename); //Loads from file

	//TODO: Asynchronous loading with multithreading - still to determine behaviour :)
	//BaseAsset* loadAsync(std::string filename);

	/*
	 * Used to retrieve a specific loader (eg. ImageLoader) in order to provide
	 * access to higher level functions within specific loaders (eg. converting to
	 * GLuint textures from raw images)
	 */
	AssetLoader* getLoader(std::string loaderName);
};


#endif //BLOCKHOP_ASSETMANAGER_H
