//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETMANAGER_H
#define BLOCKHOP_ASSETMANAGER_H


#include <string>
#include <json.hpp>
#include "assetLoader.h"
#include "imageLoader.h"
#include "../logger.h"
using json = nlohmann::json;

class AssetManager {
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

    static AssetManager *c_instance;
public:
    AssetManager();
    ~AssetManager();

    /*
     * Singleton pattern. Must use i()-> to access any class methods.
     */
    static AssetManager *i()
    {
        if (!c_instance) {
            c_instance = new AssetManager();
        }
        return c_instance;
    }

    /*
     * Load a raw asset from file. Will automatically determine the loader to use for the
     * filename/extension given and will then check if the asset is already loaded. If not
     * it will conduct a load synchronously.
     */
    BaseAsset* loadSync(std::string filename); //Loads from file
    //loadAsync();

    /*
     * Used to retrieve a specific loader (eg. ImageLoader) in order to provide
     * access to higher level functions within specific loaders (eg. converting to
     * GLuint textures from raw images)
     */
    AssetLoader* getLoader(std::string loaderName);
};


#endif //BLOCKHOP_ASSETMANAGER_H
