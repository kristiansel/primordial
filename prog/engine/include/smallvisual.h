#ifndef SMALLVISUAL_H
#define SMALLVISUAL_H

#include <memory>
#include <vector>
#include "mesh.h"
#include "texture.h"

struct SmallVisual
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> tex;
    std::vector<glm::vec4> positions;
    bool updated;
};


#endif // SMALLVISUAL_H
