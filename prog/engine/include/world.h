#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <memory.h>

#include "obstacle.h"

using namespace std;

class World
{
    public:
        World();
        virtual ~World();

        list<Obstacle>::iterator addObstacle(string mesh_key, string tex_key, vec3 pos, vec3 dir); /// Update specification later
        void delObstacle(list<Obstacle>::iterator obst_it_in);

        /// find a way to privatise this?
        list<Obstacle> obstacles;

        // void forAllObstacles( void (*f) (Obstacle&) );

    protected:
    private:


};

#endif // WORLD_H
