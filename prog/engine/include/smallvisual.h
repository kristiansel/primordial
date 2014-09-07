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

    void updatePositionsTex() const;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> tex;
    //std::vector<glm::vec4> positions;

    static const unsigned int MAX_NUM_SMVIS = 5000;
    unsigned int num_smvis;
    glm::vec4 sm_buffer[MAX_NUM_SMVIS];

    mutable bool updated;

    glm::vec4 wind_params;

    GLuint worldpos_tex_id;
};


#endif // SMALLVISUAL_H
