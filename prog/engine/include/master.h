#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include "graphics.h"
#include "renderer.h"

/**
    Philosophy of the Master class: Handles technicalities
    like window creation, directs input, swap display buffer
    and manages the interfaces between different modules that
    make up the program.
*/

using namespace std;

class Master
{
    public:
        /// Methods
        Master(); // starts everything
        virtual ~Master();
    protected:
    private:
        /// Methods
        void init();
        void loadResources();
        void mainLoop();
        void cleanUp();

        bool handleInput();
        void resizeWindow(int w, int h, bool real = true);

        /// Members
        sf::Window window;
        Renderer renderer;

        sf::Clock clock;
        float dt; // frame time in seconds

};

#endif // MASTER_H
