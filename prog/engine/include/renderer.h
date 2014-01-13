#ifndef RENDERER_H
#define RENDERER_H

#include "graphics.h"

class Renderer
{
    public:
        Renderer();
        virtual ~Renderer();

        void init(); // depends on an OpenGL context, therefore public
        void draw();
    protected:
    private:
};

#endif // RENDERER_H
