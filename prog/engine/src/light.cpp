#include "light.h"

DirLight::DirLight() :
    dir(glm::vec3(1.0, 1.0, 1.0)),
    color(glm::vec4(1.0, 0.8, 0.6, 1.0)) {};

glm::mat4 DirLight::getVPmatrix(glm::vec3 focus_pos) const
{
    //glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-40,40);
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20,20,-20,20,-40,40);
    glm::mat4 depthViewMatrix = glm::lookAt(dir + focus_pos, glm::vec3(0,0,0) + focus_pos, glm::vec3(0,1,0));

    return (depthProjectionMatrix * depthViewMatrix);
}

PointLight::PointLight() :
    pos(glm::vec3(0.0, 0.0, 0.0)),
    color(glm::vec4(1.0, 1.0, 1.0, 1.0)) {};
