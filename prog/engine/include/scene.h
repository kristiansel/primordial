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
    class Reader;
    class Writer;
public:
    Scene();
    virtual ~Scene();

    string debugInfo();

    /// Modifying methods
    list<Prop>::iterator addProp(string mesh_key, string tex_key, vec3 pos, vec3 dir); /// Update specification later
    void delProp(list<Prop>::iterator prop_it_in);

    /// Some way of making this private
    Camera camera;

    /// background color
    vec4 bg_color;



    /// to be removed
    list<Prop> props;

protected:
private:
    /// basic scene components

    // list<Actor> actors;
    // Terrain terrain;

    //ResourceManager resourcemanager;

    ResourceManager<Mesh>    mesh_manager;
    ResourceManager<Texture> tex_manager;

};

#endif // SCENE_H
