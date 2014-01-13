#include "renderer.h"

Renderer::Renderer()
{
    //ctor
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::init()
{
    /// Set clear values
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClearDepth(1.0);

    /// Enable depth testing
    glEnable(GL_DEPTH_TEST);

    /// Set up culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);
}

void Renderer::draw()
{
    /// clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
