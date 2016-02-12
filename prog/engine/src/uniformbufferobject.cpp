#include "uniformbufferobject.h"

UniformBufferObject::UniformBufferObject()
{
    //ctor
}

UniformBufferObject::~UniformBufferObject()
{
    //dtor
}

void UniformBufferObject::init()
{
    // REQUEST the UBO on the server
    //
    //
    //
    //             UBO
    //
    globalUniforms.ubo_size = sizeof(globalUniformsData);

    glGenBuffers(1, &globalUniforms.ubo_id);
    glBindBuffer(GL_UNIFORM_BUFFER, globalUniforms.ubo_id);
    glBufferData(GL_UNIFORM_BUFFER, globalUniforms.ubo_size , NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // CREATE a global Binding index for uniform buffer objects
    //
    //       Binding0
    //
    //             UBO
    //
    globalUniforms.binding_index = 0;

    // CONNECT the UBO to the Binding Index
    //
    //       Binding0
    //             |
    //             UBO
    //
    glBindBufferRange(GL_UNIFORM_BUFFER, globalUniforms.binding_index,
    globalUniforms.ubo_id, 0, globalUniforms.ubo_size);
}

GLuint UniformBufferObject::getBinding()
{
    return globalUniforms.binding_index;
}

void UniformBufferObject::setGlobalUniformsData(GlobalUniformsData in)
{
    globalUniformsData = in;

    glBindBuffer(GL_UNIFORM_BUFFER, globalUniforms.ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, globalUniforms.ubo_size, &(globalUniformsData.proj_matUni[0]));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
