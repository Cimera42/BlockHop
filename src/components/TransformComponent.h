//
// Created by Tim on 19/04/2017.
//

#ifndef BLOCKHOP_TRANSFORMCOMPONENT_H
#define BLOCKHOP_TRANSFORMCOMPONENT_H

#include "../ecs/component.h"
#include "../ecs/ecsManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class TransformComponent : public Component
{
    static bool exported;
public:
    TransformComponent();
    ~TransformComponent();
    void setValues(json inValues);
    
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    friend std::ostream &operator<< (std::ostream &os, TransformComponent const &t) {
        os << "TransformComponent: ";
        os << "(" << t.position.x << ", " << t.position.y << ", " << t.position.z << "), ";
        os << "(" << t.rotation.w << ", " << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << "), ";
        os << "(" << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << "), ";
        return os;
    }
};


#endif //BLOCKHOP_TRANSFORMCOMPONENT_H
