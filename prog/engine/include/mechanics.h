#ifndef MECHANICS_H
#define MECHANICS_H

#include <sstream>
#include "world.h"

/// For testing
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "signals.h"

class Mechanics
{
public:
    Mechanics();
    virtual ~Mechanics();

    /// Issue: What should be the division of tasks between
    /// Mechanics and world?

    /// world->addCreature OK
    /// but world->Step(dt) not sure

    /// Input/AI = WorldCreature signals (stored on each creature "ActionRequests")
    /// Resolve these requests (game "mechanics") to find the active "Action"
    /// "Action" determines forces (for physics) and "Animation State"
    /// Forces acted out together with hit testing for Combat
    /// These produce ActionRequests for the next frame, for example if someone was
    /// hit, this takes precedence over their own AI wishes

    void init(World &world_in, float &dt_in); /// remove the dt after a while
    void step(World &world_in, float dt_in);

    string debugInfo();

    /// External controls
    void playerMoveForward();
    void playerMoveBackward();
    void playerMoveLeft();
    void playerMoveRight();

    void playerRotateUp();
    void playerRotateDown();
    void playerRotateRight();
    void playerRotateLeft();

    void playerRotateUpVal(float);
    void playerRotateLeftVal(float);

    /// Testing
    void func(int num_in);
protected:
    void toggleCamera();
private:
    World *world;
    float* dt; /// refactor away

    /// Controls related
    const float speed;          /// 1.0/s
    const float camTurnSpeed;   /// deg/s
    MoveSignalReceiver* controlled;

    /// Temporary for testing deletion
    list<shared_ptr<WorldObject>>::iterator worldobject_ptr_it;

    /// Some tricks:
//    class MovableAbstrBase
//    {
//    public:
//        virtual void moveForward() = 0;
//        virtual void moveBackward() = 0;
//        virtual void moveLeft() = 0;
//        virtual void moveRight() = 0;
//
//        virtual void rotateUp() = 0;
//        virtual void rotateDown() = 0;
//        virtual void rotateRight() = 0;
//        virtual void rotateLeft() = 0;
//
//        virtual void rotateUpVal(float) = 0;
//        virtual void rotateLeftVal(float) = 0;
//    };
//
//    class CamMover : public MovableAbsBase
//    {
//        friend Mechanics
//        void moveForward();
//        void moveBackward();
//        void moveLeft();
//        void moveRight();
//
//        void rotateUp();
//        void rotateDown();
//        void rotateRight();
//        void rotateLeft();
//
//        void rotateUpVal(float);
//        void rotateLeftVal(float);
//    } freeLook;
//
//    class ThirdPerson : public MovableAbsBase
//    {
//        ThirdPerson(Mechanics* mech_in) {this->mech = mech_in;};
//        void moveForward();
//        void moveBackward();
//        void moveLeft();
//        void moveRight();
//
//        void rotateUp();
//        void rotateDown();
//        void rotateRight();
//        void rotateLeft();
//
//        void rotateUpVal(float);
//        void rotateLeftVal(float);
//
//        Mechanics* mech;
//    } thirdPerson;
};

#endif // MECHANICS_H
