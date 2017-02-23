#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include "angle.h"

class Camera
{
    public:
        Camera();
        ~Camera();

        Angle pitch, yaw;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        glm::vec3 * posLink;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        void lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up);
        void perspective(float fov, float aspectRatio, float nearDist, float farDist);

        void link(glm::vec3* inPosLink);
        void unlink();
        void update();
};

#endif // CAMERA_H_INCLUDED
