#include "debugdrawer.h"

DebugDrawer::DebugDrawer()
{
    //ctor
}

DebugDrawer::~DebugDrawer()
{
    //dtor
}

void DebugDrawer::drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    glLineWidth(2.5);
    glColor3f((float)color[0], (float)color[1], (float)color[2]);
    glBegin(GL_LINES);
    glVertex3f((float)from[0], (float)from[1], (float)from[2]);
    glVertex3f((float)to[0], (float)to[1], (float)to[2]);
    glEnd();
}
