#ifndef CAMERA_H
#define CAMERA_H

#include "object3d.h"

/// Beware! The camera perspective properties are handled by the Renderer class
/// This "Camera" is meant only as a world component

class Camera: public Object3d
{
public:
    Camera();
    virtual ~Camera();

    mat4 getModelViewMatrix();
};

#endif // CAMERA_H
