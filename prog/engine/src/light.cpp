#include "light.h"

DirLight::DirLight() :
    dir(glm::vec3(1.0, 1.0, 1.0)),
    color(glm::vec4(1.0, 0.8, 0.6, 1.0)) {};

glm::mat4 DirLight::getVPmatrix() const
{
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(dir, glm::vec3(0,0,0), glm::vec3(0,1,0));

    return (depthProjectionMatrix * depthViewMatrix);
}

PointLight::PointLight() :
    pos(glm::vec3(0.0, 0.0, 0.0)),
    color(glm::vec4(1.0, 1.0, 1.0, 1.0)) {};
