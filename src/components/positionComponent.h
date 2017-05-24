//
// Created by Tim on 19/04/2017.
//

#ifndef BLOCKHOP_CLION_POSITIONCOMPONENT_H
#define BLOCKHOP_CLION_POSITIONCOMPONENT_H

#include "../ecs/component.h"
#include "../ecs/ecsManager.h"
#include <glm/glm.hpp>

class PositionComponent : public Component
{
public:
    PositionComponent();
    ~PositionComponent();
    void setValues(json inValues);

    glm::vec3 position;

    friend std::ostream &operator<< (std::ostream &os, PositionComponent const &p) {
        os << "PositionComponent: (" << p.position.x;
        os << ", " << p.position.y;
        os << ", " << p.position.z << ")";
        return os;
    }
};


#endif //BLOCKHOP_CLION_POSITIONCOMPONENT_H
