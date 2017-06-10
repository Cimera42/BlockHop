//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETMANAGER_H
#define BLOCKHOP_ASSETMANAGER_H


#include <string>
#include <json.hpp>
#include "assetLoader.h"
#include "textureLoader.h"
#include "../logger.h"
using json = nlohmann::json;

class AssetManager {
    /*
     * Base asset loading class - singleton pattern
     * Maintains a list of all assets that have been loaded into the engine
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
    std::vector<std::pair<std::string, AssetLoader*> > extLoaders; //Loader for each extension
    std::map<std::string, AssetLoader*> exportedLoaders;

    static AssetManager *c_instance;
public:
    AssetManager();
    ~AssetManager();

    void start() {
        //TODO This is just a test in order to start the loader whilst
        // we don't have loading inside the ECS Manager implementet!
        Logger() << "yes" <<std::endl;
        exit(9);
    }

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

    //loadSynch();
    //loadAsynch();

};


#endif //BLOCKHOP_ASSETMANAGER_H
