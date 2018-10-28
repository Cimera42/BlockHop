//
// Created by Jono on 17/08/2018.
//

#ifndef BLOCKHOP_CONFIGASSET_H
#define BLOCKHOP_CONFIGASSET_H

#include "baseAsset.h"
#include <json.hpp>
using json = nlohmann::json;

class ConfigAsset : public BaseAsset
{
	friend class ConfigLoader;
	bool load();
	ConfigAsset(std::string filename);
	~ConfigAsset();
public:
	// TODO move save to private if we decide to move saving to loader based approach
	bool save();
	json config;
};


#endif //BLOCKHOP_CONFIGASSET_H
