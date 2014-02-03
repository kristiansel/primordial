#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <SFML/Window.hpp>
#include "mechanics.h"
#include "renderer.h"
#include "culler.h"

/**
    Philosophy of the Master class: Handles technicalities
    like window creation, directs input, swap display buffer
    and manages the interfaces between different modules that
    make up the program.
*/

class Master
{
public:
    /// Methods
    Master(); // starts everything
    virtual ~Master();
protected:
private:
    /// Primary Methods
    void init();
    void loadResources();
    void mainLoop();
    void cleanUp();

    /// Secondary Methods
    bool handleInput();

    /// Members
    sf::Window  window;     /// Input from the window signals the game mechanics
    Mechanics   mechanics;  /// the game mechanics takes the input and transforms the scene
    World       world;      /// struct like class - Contains the game state
    Culler      culler;      /// process oriented class, scene = culler.stage(&world)
    Scene       scene;      /// the scene is an interface between the world and the renderer
    Renderer    renderer;   /// the renderer draws the scene: renderer.draw(&scene)

    /// replace by settings
    unsigned int scr_width_px;
    unsigned int scr_height_px;


    sf::Clock clock;    // helper
    float     dt;       // frame time in seconds

    bool has_focus;     // helper
};

#endif // MASTER_H
