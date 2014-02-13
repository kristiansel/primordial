#include "mechanics.h"

Mechanics::Mechanics() : speed(2.5), camTurnSpeed(60.0)
{
    //ctor
}

Mechanics::~Mechanics()
{
    //dtor
}

void Mechanics::init(World &world_in, float &dt_in)
{
    world = &world_in;
    dt = &dt_in;

    world->camera->pos = vec3(0.0, 1.5, 0.0);

    /// Load some resources (should be moved)
    ///                     Model,          Texture,        Position,               Direction
    /// WorldObjects:
    world->addDynamicObject(     "sphere",       "grass_equal",  vec3(-2.0, 10.0, -4.0) );
    world->addDynamicObject(     "sphere",       "grass_equal",  vec3(2.0, 10.0, -4.0) );
    world->addStaticObject(     "ground",         "nicewall",     vec3(0.0, 0.0, 0.0) );

    /// Creatures
    world->addCreature(     "anim_test",    "checkers",     vec3(0.0, 0.0, -4.0) );
    //world->addCreature(     "sphere",       "asdasdasd",    vec3(-3.0, 0.0, -2.0),  vec3(0.0, 0.0, -1.0));

    // world->addWorldObject(     "mdl_human_male",     "tex_human_male", vec3(-3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));

    //auto prop4 = scene->addProp(     "mdl_uv_sphere",   "checkers", vec3(0.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "checkers", vec3(3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "grass_equal", vec3(-3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));


    /// remove later
    worldobject_ptr_it = world->worldobjects.begin();

    /// Testing

//    mat4 tr = glm::transpose(Transform::translate(1.0, 2.0, 3.0));
//    mat4 rt = glm::transpose(Transform::objectRot(vec3(0.0, 1.0, 0.0), vec3(0.267261, 0.534522, 0.801784)));
//    mat4 sc = glm::transpose(Transform::scale(1.0, 2.0, 3.0));
//    mat4 vertex_matrix  = tr * rt * sc; // scale, then translate, then lookat.
//    mm::matPrint(vertex_matrix);
//
//    mat4 tr2 = glm::translate(mat4(1.0), vec3(1.0, 2.0, 3.0));
//    mat4 rt2 = glm::transpose(Transform::objectRot(vec3(0.0, 1.0, 0.0), vec3(0.267261, 0.534522, 0.801784)));
//    mat4 sc2 = glm::scale(mat4(1.0), vec3(1.0, 2.0, 3.0));
//    mat4 vertex_matrix2  = tr2 * rt2 * sc2; // scale, then translate, then lookat.
//    mm::matPrint(vertex_matrix2);

//    mat4 tr2 = glm::translate(mat4(1.0), vec3(1.0, 2.0, 3.0));
//    mat4 rt2 = glm::transpose(Transform::objectRot(vec3(0.0, 1.0, 0.0), vec3(0.267261, 0.534522, 0.801784)));
//    mat4 mv2 = glm::inverse(tr2 * rt2);
//    mm::matPrint( mv2 );
//
//    mat4 mv = Transform::lookAt(vec3(1.0, 2.0, 3.0) , vec3(0.267261, 0.534522, 0.801784), vec3(0.0, 1.0, 0.0)) ;
//    mv = glm::transpose(mv) ;
//    mm::matPrint(mv);

//     mm::matPrint(glm::perspective(0.785398163f, 4.0f / 3.0f, 0.1f, 100.f));
//     mm::matPrint(Transform::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f));
}

void Mechanics::step(World &world_in, float dt_in)
{
    //world_in.step(dt_in);
    world_in.physicsStep(dt_in);
    for (shared_ptr<WorldObject> wObject : world_in.worldobjects)
    {
        wObject->updateTransformation();
    }
}

string Mechanics::debugInfo()
{
    std::ostringstream output;
    output  << "Debugging mechanics: \n"
            << "dt = " << *dt << "\n"
            << "speed = " << speed << "\n";
    return output.str();
}

/// move
void Mechanics::playerMoveForward()
{
    world->camera->moveForward((*dt)*speed);
}

void Mechanics::playerMoveBackward()
{
    world->camera->moveForward(-(*dt)*speed);
}

void Mechanics::playerMoveLeft()
{
    world->camera->strafeLeft((*dt)*speed);
}

void Mechanics::playerMoveRight()
{
    world->camera->strafeLeft(-(*dt)*speed);
}


/// rotate
void Mechanics::playerRotateUp()
{
    world->camera->panUp((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateDown()
{
    world->camera->panUp(-(*dt)*camTurnSpeed);
}

void Mechanics::playerRotateLeft()
{
    world->camera->panLeft((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateRight()
{
    world->camera->panLeft(-(*dt)*camTurnSpeed);
}

void Mechanics::func(int num_in)
{
    switch (num_in)
    {
    case 1:
        if (world->worldobjects.empty())
        {
            worldobject_ptr_it = world->addDynamicObject(     "sphere",   "grass_equal", world->camera->pos + 2.f*world->camera->getDir() );
        }
        else
        {
            world->addDynamicObject(     "sphere",   "grass_equal", world->camera->pos + 2.f*world->camera->getDir() );
        }
        break;
    case 2:     world->delWorldObject(worldobject_ptr_it); worldobject_ptr_it = world->worldobjects.begin(); break;
    default:    break;
    }
}
