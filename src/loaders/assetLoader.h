//
// Created by Jono on 10/06/2017.
//

#ifndef BLOCKHOP_ASSETLOADER_H
#define BLOCKHOP_ASSETLOADER_H

#include <map>
#include <string>
#include "baseAsset.h"

class AssetLoader {
protected:
    std::string loaderType;
public:
    /*
     * loadAsset must be implemented by a loader class
     * which will ensure loading is completed. It is recommended
     * this is done synchronously in here.
     */
    virtual BaseAsset* loadAsset(std::string filename);

    //virtual BaseAsset* getDefaultAsset();

    /*
     * An asset is deemed to exist if it is either loaded
     * or in the process of loading.
     */
    virtual bool assetExists(std::string filename);

    /*
     * Finds an asset that is loaded, or wait until it is
     * loaded if it is in the process of loading.
     */
    virtual BaseAsset* findAsset(std::string filename);
};


#endif //BLOCKHOP_ASSETLOADER_H
