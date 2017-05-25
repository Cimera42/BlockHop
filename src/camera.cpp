#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

Camera::Camera()
{
    pitch = Angle(true, -3.1415/2.0, 3.1415/2.0);
    yaw = Angle();
}
Camera::~Camera()
{

}

void Camera::lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
{
    viewMatrix = glm::lookAt(eye, centre, up);
}

void Camera::perspective(float fov, float aspectRatio, float nearDist, float farDist)
{
    projectionMatrix = glm::perspective(fov, aspectRatio, nearDist, farDist);
}

void Camera::link(glm::vec3* inPosLink)
{
    posLink = inPosLink;
}
void Camera::unlink()
{
    posLink = nullptr;
}

void Camera::update()
{
    forward = glm::vec3(cos(pitch) * sin(yaw),
                        sin(pitch),
                        cos(pitch) * cos(yaw));

    right = glm::vec3(sin(yaw - 3.14f/2.0f),
                      0,
                      cos(yaw - 3.14f/2.0f));
    up = glm::cross(right, forward);

    glm::vec3 tempPos(0,0,0);
    if(posLink)
    {
        tempPos = *posLink;
    }

    lookAt(tempPos,
           tempPos + forward,
           up);
}
