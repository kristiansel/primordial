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

    glm::mat4 getModelViewMatrix(); /// Consider eliminating the camera class
                               /// altogether, make a method within renderer to
                               /// compute a modelview matrix from a (pos, dir)
};

#endif // CAMERA_H
