//
// Created by Jono on 17/08/2018.
//

#include "configAsset.h"
#include "../logger.h"
#include <fstream>

ConfigAsset::ConfigAsset(std::string filename): BaseAsset(filename) {}

ConfigAsset::~ConfigAsset() {}

bool ConfigAsset::load()
{
	std::ifstream i(filename);
	try {
		if (i.is_open()) {
			i >> config;
		}
	} catch (std::invalid_argument invalidArgument) {
		Logger() << invalidArgument.what();
		Logger()<<"Cannot read config file, "+filename<<std::endl;
		return false;
	} catch (...) {
		Logger()<<"Cannot read config file, "+filename<<std::endl;
		return false;
	}

	if(!config)
	{
		Logger() << "Could not load config: " << filename;
		return false;
	}
	return true;
}

bool ConfigAsset::save() {
	// TODO set immutability by default, unless we unset
	// ie. prevent people changing config at all, not just saving
	if (isMutable) {
		std::ofstream o(filename);
		o << config << std::endl;
		Logger()<<filename<<" saved."<<std::endl;
		return true;
	}
	Logger()<<filename<<" is set to immutable and cannot be saved."<<std::endl;
	return false;
}
