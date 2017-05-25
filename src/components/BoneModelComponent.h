//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_BONEMODELCOMPONENT_H
#define BLOCKHOP_BONEMODELCOMPONENT_H

#include "../ecs/component.h"

class BoneModelComponent : public Component
{
    static bool exported;
public:
    BoneModelComponent();
    ~BoneModelComponent();
    void setValues(json inValues);

    friend std::ostream &operator<< (std::ostream &os, BoneModelComponent const &c) {
        os << "BoneModelComponent";
        return os;
    }
};

#endif //BLOCKHOP_BONEMODELCOMPONENT_H
