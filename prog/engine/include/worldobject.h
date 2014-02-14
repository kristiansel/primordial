#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "prop.h"
#include "rigidbody.h"

class WorldObject : public Prop, public RigidBody
// class WorldObject : public Prop
{
    public:
        WorldObject();
        virtual ~WorldObject();

        /// WorldObject is defined by:
        /// Model               GFX             string/enum
        /// Texture             GFX             string/enum
        /// Material            GFX             Material
        /// position            GFX/PHY         vec3
        /// rotation            GFX/PHY         quat <- should be otional
        /// scale               GFX             vec3 <- consider cutting
        /// Collision shape     PHY             enum

    protected:
    private:
};

#endif // OBSTACLE_H
