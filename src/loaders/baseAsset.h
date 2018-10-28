//
// Created by Jono on 12/06/2017.
//

#ifndef BLOCKHOP_BASEASSET_H
#define BLOCKHOP_BASEASSET_H

#include <string>

class BaseAsset {
	friend class AssetLoader;
	virtual bool load();
	// Be careful with save functionality. Only want to use it in scene editors or client side configs
	virtual bool save();
protected:
	std::string filename = "";
	bool isMutable = false;
public:
	BaseAsset(std::string filename);
	virtual ~BaseAsset();
	std::string getName();
	void setMutable(bool v);
};


#endif //BLOCKHOP_BASEASSET_H
