#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <SFML/Window.hpp>
#include "mechanics.h"
#include "renderer.h"
#include "culler.h"
#include "threadingwrapper.h"

/**
    Philosophy of the Master class: Handles technicalities
    like window creation, directs input, swap display buffer
    and manages the interfaces between different modules that
    make up the program.
*/

class Master
{
public:
    // Methods
    Master(); // starts everything
    virtual ~Master();
protected:
private:
    // Primary Methods
//    void init();
    void initWindow();
    void mainLoop();
    void cleanUp();
    void renderTasks();
//    void testThreadTasks();

    // Secondary Methods
    bool handleInput();

    // Members
    sf::Window  window;     // Input from the window signals the game mechanics
    Mechanics   mechanics;  // the game mechanics takes the input and transforms the scene
    World       world;      // struct like class - Contains the game state
    Culler      culler;     // process oriented class, scene = culler.stage(&world)
    Scene       scene;      // the scene is an interface between the world and the renderer
    Renderer    renderer;   // the renderer draws the scene: renderer.draw(&scene)

    // replace by settings
    unsigned int scr_width_px;
    unsigned int scr_height_px;

    // Time-related

    sf::Clock clock;    // helper
    sf::Clock absClock; // never reset
    float     dt;       // frame time in seconds
    float      t;
    bool running;

    // Threads
    Thread render_thread;
//    Thread test_thread;

    // Thread wait flags
    bool render_thread_loaded;
    bool main_thread_loaded;

    // HoopJumping required to call XInitThreads() on unix before sf::Window
    //int init_threads;

    // Internal variable signals
    bool has_focus;     // helper
    bool rmb_down;
    int press_pos_x;
    int press_pos_y;
};

#endif // MASTER_H
