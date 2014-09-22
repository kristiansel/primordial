#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <SFML/Window.hpp>
#include "mechanics.h"
#include "renderer.h"
//#include "culler.h"
#include "threadingwrapper.h"
#include "x_init_threads.h"
#include "threadiobuffers.hpp"
#include "backgroundmaster.h"

// move later
#include "quadtree.hpp"

#ifdef WINDOWS
#include "windows.h"
#endif // WINDOWS

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

    int restart(); // whether to restart 1 or not 0

protected:
private:
    // Primary Methods
//    void init();
    void initWindow();
    void mainLoop();
    void mainLoopSingleThreaded();
    void cleanUp();
    void renderTasks();
//    void testThreadTasks();

    void backGroundTasks();



    // Secondary Methods
    bool handleInput();

    // Members
    Renderer    renderer;   // the renderer draws the scene: renderer.draw(&scene)
    sf::Window  window;     // Input from the window signals the game mechanics
    Mechanics   mechanics;  // the game mechanics takes the input and transforms the scene
    World       world;      // struct like class - Contains the game state

    // bottom gets deleted first (world before mechanics, so creatures can remove their own AI

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
    PrimT::Thread render_thread;
//    Thread test_thread;

    BackGroundMaster bg_thread;

    // Thread wait flags
    bool render_thread_loaded;
    bool main_thread_loaded;

    ThreadIObuffers<char, 200> main_bgr_iobuffer;

    PrimT::Thread back_ground;

    // HoopJumping required to call XInitThreads() on unix before sf::Window
    //int init_threads;

    // Internal variable signals
    bool has_focus;     // helper
    bool rmb_down;
    int press_pos_x;
    int press_pos_y;

    void resetPressPos()
    {
        press_pos_x = scr_width_px/2;
        press_pos_y = scr_height_px/2;
        sf::Vector2i mouse_pos = sf::Vector2i(press_pos_x, press_pos_y);
        sf::Mouse::setPosition(mouse_pos, window);
    };

    int m_restart;
};

#endif // MASTER_H
