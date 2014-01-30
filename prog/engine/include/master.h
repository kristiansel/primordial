#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <SFML/Window.hpp>
#include "mechanics.h"
#include "scene.h"
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

    /// Members
    sf::Window window;      /// Input from the window signals the game mechanics
    Mechanics mechanics;    /// the game mechanics takes the input and transforms the scene
    Scene scene;            /// the scene acts as an interface between the game and the renderer
    Renderer renderer;      /// the renderer draws the current scene using OpenGL

    /// replace by settings
    unsigned int scr_width_px;
    unsigned int scr_height_px;


    sf::Clock clock; // helper
    float dt; // frame time in seconds

    bool has_focus; // helper

};

#endif // MASTER_H
