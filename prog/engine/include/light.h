#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct DirLight
{
    // Methods
    DirLight();
    glm::mat4 getVPmatrix(glm::vec3 focus_pos) const;

    // Members
    glm::vec3 dir;
    glm::vec4 color;
};

struct PointLight
{
    PointLight();
    // Later add method of creating FOUR view-projection matrices
    // for shadowing point light

    // Members
    glm::vec3 pos;
    glm::vec4 color;
};

#endif // LIGHT_H
