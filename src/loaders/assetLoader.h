//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETLOADER_H
#define BLOCKHOP_ASSETLOADER_H

#include <map>
#include <string>
#include <vector>
#include "baseAsset.h"

class AssetLoader {
    friend class AssetManager;
protected:
    std::string loaderType;
    //Buffer of assets that are currently loading
    std::vector<std::string> inLoad;
    //Array of all assets that are loaded
    std::map<std::string, BaseAsset*> fileList; //Raw image data
    //A default asset that is gauranteed to exist
    BaseAsset* defaultAsset;

    /*
    * loadAsset will ensure loading is completed.
    * It is recommended this is done synchronously.
    */
    BaseAsset* loadAsset(std::string filename, BaseAsset* newAsset);
public:
    AssetLoader();
    virtual ~AssetLoader();

    /*
     * Retrieves the default asset for a loader
     */
    BaseAsset* getDefaultAsset() const;

    /*
     * loadAsset must be overwritten to provide an asset type to load
     * Will need to create a new asset of type X and pass to protected
     * loadAsset
     */
    virtual BaseAsset* loadAsset(std::string filename);

    /*
     * An asset is deemed to exist if it is either loaded
     * or in the process of loading.
     */
    bool assetExists(std::string filename);

    /*
     * Finds an asset that is loaded, or wait until it is
     * loaded if it is in the process of loading.
     */
    BaseAsset* findAsset(std::string filename);
};


#endif //BLOCKHOP_ASSETLOADER_H
