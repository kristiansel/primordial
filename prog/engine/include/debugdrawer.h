#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H

#include "LinearMath/btIDebugDraw.h"
#include "GL/gl.h"

class DebugDrawer : public btIDebugDraw
{
    public:
        DebugDrawer();
        virtual ~DebugDrawer();

        void drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color);

        void drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) {}

        void reportErrorWarning (const char *warningString) {}

        void draw3dText (const btVector3 &location, const char *textString) {}

        void setDebugMode (int debugMode) {}

        int getDebugMode () const {return 0; }

    protected:
    private:
};

#endif // DEBUGDRAWER_H
