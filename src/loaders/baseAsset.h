//
// Created by Jono on 12/06/2017.
//

#ifndef BLOCKHOP_BASEASSET_H
#define BLOCKHOP_BASEASSET_H

#include <string>

class BaseAsset {
	friend class AssetLoader;
	virtual bool load();
protected:
	std::string filename = "";
public:
	explicit BaseAsset(const std::string &filename);
	virtual ~BaseAsset();
	std::string getName();
};


#endif //BLOCKHOP_BASEASSET_H
