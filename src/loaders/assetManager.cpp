//
// Created by Jono on 10/06/2017.
//

#include "assetManager.h"
#include "../logger.h"
#include <fstream>

AssetManager *AssetManager::c_instance = 0;

AssetManager::AssetManager() {

    //TODO possibly replace this with non-hard? Is it worth it?
    exportedLoaders.insert(std::pair<std::string, AssetLoader*>("texture", new ImageLoader()));

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
            for(json extension : loaderType["extensions"]) {
                std::string ext = extension.get<std::string>();
                auto it = exportedLoaders.find(type);
                if(it == exportedLoaders.end()) {
                    Logger(1) << "Loader for '" << ext
                              << "' doesn't exist. Assets of this type will not work correctly"<<std::endl;
                }
                else {
                    extLoaders.insert(std::pair<std::string, AssetLoader*>(ext, it->second));
                    //TODO load the default item synchronously
                }
            }
        }
    }catch (std::invalid_argument invalidArgument) {
        Logger(1) << invalidArgument.what();
        Logger(1)<<"Cannot read config file, "+config+". AssetLoader will not function correctly."<<std::endl;
        exit(-6);
    }catch (...) {
        Logger(1)<<"Cannot read config file, "+config+". AssetLoader will not function correctly."<<std::endl;
        exit(-6);
    }

    i.close();
}

void AssetManager::loadSync(std::string filename) {
    //Determine file extension
    auto dotPos = filename.find_last_of(".");
    std::string ext = filename.substr(dotPos+1);

    //Attempt to find file extension in map to get specific loader
    auto extLoader = extLoaders.find(ext);

    //Load using loader
    if(extLoader != extLoaders.end()) {
        extLoader->second->loadAsset(filename);
    } else {
        Logger(1)<<"Could not find a loader for the file '"<<filename<<"'."<<std::endl;
    }
}

AssetLoader* AssetManager::getLoader(std::string loaderName) {
    //Find within exportedLoaders
    auto it = exportedLoaders.find(loaderName);

    if(it != exportedLoaders.end()) {
        return it->second;
    } else {
        Logger(1)<<"Cannot find loader with given name '"<<loaderName<<"', ensure loader exists"
                "inside asset configuration file."<<std::endl;
    }
    return nullptr;
}