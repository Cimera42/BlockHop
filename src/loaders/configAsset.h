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
	bool save();
	ConfigAsset(std::string filename);
	~ConfigAsset();
	bool isMutable = false; //TODO change once implemented save
public:
	json config;
};


#endif //BLOCKHOP_CONFIGASSET_H
