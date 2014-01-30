#ifndef SCENE_H
#define SCENE_H

#include <sstream>
#include <list>
#include "camera.h"
#include "mesh.h" /// remove
#include "resourcemanager.h"
#include "prop.h"


using namespace std;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    string debugInfo();


    /// Some way of making this private
    Camera camera;


    list<Prop>::iterator addProp(string mesh_key, vec3 pos, vec3 dir); /// Update specification later
//        void delProp(list<Prop>::iterator it_in);
//
//        list<Actor>::iterator addActor(some actor specification...);
//        void delActor(list<Actor>::iterator it_in);



    /// to be removed
    list<Mesh> meshes;
    list<Prop> props;

protected:
private:
    /// basic scene components

    // list<Actor> actors;
    // Terrain terrain;

    ResourceManager resourcemanager;

};

#endif // SCENE_H
