//
// Created by Jono on 12/06/2017.
//

#ifndef BLOCKHOP_BASEASSET_H
#define BLOCKHOP_BASEASSET_H

#include <string>

class BaseAsset {
protected:
    std::string filename = "";
public:
    BaseAsset(std::string filename);
    virtual ~BaseAsset();
    virtual void load();
    std::string getName();
};


#endif //BLOCKHOP_BASEASSET_H
