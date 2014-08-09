#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <memory.h>

#include "worldobject.h"
#include "creature.h"
// #include "resourcemanager.h"
#include "camera.h"
#include "physicsworld.h"
#include "light.h"
#include "terrain.h"

#include "global.h"


using std::list;
using std::shared_ptr;
using std::weak_ptr;
using std::string;

class World : public PhysicsWorld // Consider merging Culling functionality into World Class
{
    public:
        World();
        virtual ~World();

        list<shared_ptr<WorldObject>>::iterator addStaticObject(string mesh_key,
                                                                string tex_key,
                                                                glm::vec3 pos);

        list<shared_ptr<WorldObject>>::iterator addDynamicObject(string mesh_key,
                                                                 string tex_key,
                                                                 glm::vec3 pos,
                                                                 btCollisionShape* shape = nullptr);

        void delWorldObject(list<shared_ptr<WorldObject>>::iterator obst_it_in);

        list<shared_ptr<Creature>>::iterator addCreature(string mesh_key,
                                                         string tex_key,
                                                         glm::vec3 pos);

        void delCreature(list<shared_ptr<Creature>>::iterator creature_it_in);

        void mainLight(glm::vec3 dir, glm::vec4 color );

        void addTerrain();

//        // Step
//        void step(float dt_in);

        void startMusic(string soundKey);
        sf::Music* music;



        // "Physical" Contents (could with benefit be private?)
        list<shared_ptr<WorldObject>>    worldobjects;
        list<shared_ptr<Creature>>       creatures;
        // list<shared_ptr<Light>>       lights; // For future
        Terrain                          terrain;

        //shared_ptr<Camera> camera; // Shared pointer here, because we do not want other shared
        // pointers to accidentally delete the camera.

        // Safe pointers are for wimps...
        Camera *freecam;    // Owning pointer
        Camera *chasecam;   // Owning pointer
        Camera const *active_cam; // Non-owning pointer (points to either above)

        DirLight* main_light;

        // Point lights
        static const int MAX_NUM_POINT_LIGHTS = 5;
        int num_point_lights;
        PointLight* point_lights;


        // void forAllWorldObjects( void (*f) (WorldObject&) );

    protected:
    private:

};

#endif // WORLD_H
