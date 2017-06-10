//
// Created by Jono on 10/06/2017.
//

#include "assetManager.h"
#include "../logger.h"
#include <fstream>

AssetManager *AssetManager::c_instance = 0;

AssetManager::AssetManager() {

    //TODO possibly replace this with non-hard? Is it worth it?
    exportedLoaders.insert(std::pair<std::string, AssetLoader*>("texture", new TextureLoader()));

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
                    extLoaders.push_back(std::pair<std::string, AssetLoader*>(ext, it->second));
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