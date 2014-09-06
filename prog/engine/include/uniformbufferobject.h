#ifndef UNIFORMBUFFEROBJECT_H
#define UNIFORMBUFFEROBJECT_H

#include "geometry.h"

struct UniformBufferObject
{
    UniformBufferObject();
    ~UniformBufferObject();

    void init();

    GLuint getBinding();

    struct GlobalUniformsData;
    void setGlobalUniformsData(GlobalUniformsData in);


    struct BufferObjectInstance
    {
        GLuint ubo_id;
        size_t ubo_size;
        GLuint binding_index;
        GLuint block_index;
    };


    BufferObjectInstance globalUniforms;
    struct GlobalUniformsData
    {
        glm::mat4 proj_matUni;
        glm::vec4 fog_colorUni;
        glm::vec4 sky_colorUni;
        glm::vec4 main_light_colorUni;
        glm::vec3 main_light_dirUni;         // does this align?
        float zfarUni;
        glm::vec4 wind_speed;
    } globalUniformsData;
};

#endif // UNIFORMBUFFEROBJECT_H
