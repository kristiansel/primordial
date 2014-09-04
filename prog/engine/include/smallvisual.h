#ifndef SMALLVISUAL_H
#define SMALLVISUAL_H

#include <memory>
#include <vector>
#include "mesh.h"
#include "texture.h"

struct SmallVisual
{
    SmallVisual();
    ~SmallVisual();

    void init();
    void cleanUp();

    void updatePositionsTex();

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> tex;
    std::vector<glm::vec4> positions;
    bool updated;

    GLuint worldpos_tex_id;
};


#endif // SMALLVISUAL_H
