#ifndef SCENE_H
#define SCENE_H

#include <list>

#include "prop.h"
#include "actor.h"
#include "terrain.h"
#include "camera.h"
#include "light.h"
#include "smallvisual.h" // remove this

class Scene
{
    public:
        Scene();
        virtual ~Scene();

//       Scene& operator=(Scene&& other);

        // why not shared pointers? Could consider, but do not want
        // to risk accidental deletion, in that case, could make original
        // container use shared pointers
        std::list<std::shared_ptr<Prop>> props;
        std::list<std::shared_ptr<Actor>> actors;
        std::vector<SmallVisual>* small_visuals;

        Terrain* terrain;

        //std::shared_ptr<Camera> camera;
        Camera const *camera;

        const DirLight* main_light;

    protected:
    private:
};

#endif // SCENE_H
