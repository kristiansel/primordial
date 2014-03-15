#include "master.h"

Master::Master() :
    dt(0.0),
    has_focus(true),
    rmb_down(false),
    press_pos_x(0),
    press_pos_y(0),
    running(true),
    render_thread_loaded(false)
{
//    /// initiate
//    init();
//    /// initiate
//    init();

    /// load resources
    loadResources();

    /// Start window in main thread...
    initWindow();

    /// Launch threads
    render_thread = Thread(&Master::renderTasks, this);
    //test_thread = Thread(&Master::testThreadTasks, this);

    /// Wait for render_thread to load fully
    std::cout << "waiting for render thread\n";
    while (!render_thread_loaded) {} /// WAIT

    /// main loop
    mainLoop();

    /// cleanup
    cleanUp();
}

Master::~Master()
{
    /// dtor
}

void Master::initWindow()
{
    /// to be loaded from settings
    scr_width_px = 1400;
    scr_height_px = 900;

    /// create the window and OpenGL context
    window.create(sf::VideoMode(scr_width_px, scr_height_px), "Primordial", sf::Style::Default, sf::ContextSettings(32, 0, 0, 4, 4));
    window.setVerticalSyncEnabled(false); /// This forces frame rate to 60 FPS?

    window.setActive(false);

    // Disable repeated key presses when holding down keys
    window.setKeyRepeatEnabled(false);
}

//void Master::init()
//{
////    /// to be loaded from settings
////    scr_width_px = 1400;
////    scr_height_px = 900;
////
////    /// create the window and OpenGL context
////    window.create(sf::VideoMode(scr_width_px, scr_height_px), "Primordial", sf::Style::Default, sf::ContextSettings(32, 0, 0, 4, 4));
////    window.setVerticalSyncEnabled(true);
////
////    window.setActive(false);
////    /// Deactivate window in current thread
////    window.setActive(true);
////
////    renderer.init(scr_width_px, scr_height_px);
////
////    /// Initialize the game module
////    mechanics.init(world, dt);
//
//
//}

void Master::loadResources()
{

}

void Master::mainLoop()
{
    /// Initialize the game module
    mechanics.init(world, dt);


    /// run the main loop
    while (running)
    {
        /// start the watch
        clock.restart();

        /// handle user input
        running = handleInput();

        /// Step the simulation
        mechanics.step(world, dt);

        // cout << "dt = " << dt << "\n";

//        /// Choose what to render
//        culler.stage(scene, world); /// stage the scene from the world THIS breaks everything since non-shared shared pointers go out of scope
//
//        /// draw...
//        renderer.draw(scene, dt);
//
//        /// end the current frame (internally swaps the front and back buffers)
//        window.display();

        /// record the time
        dt = clock.getElapsedTime().asSeconds();

//        cout << "frame time = " << dt << ", FPS = " << 1.0/dt << "\n";
//        system("PAUSE");
    }
}

void Master::renderTasks()
{
//    /// to be loaded from settings
//    scr_width_px = 1400;
//    scr_height_px = 900;
//
//    /// create the window and OpenGL context
//    window.create(sf::VideoMode(scr_width_px, scr_height_px), "Primordial", sf::Style::Default, sf::ContextSettings(32, 0, 0, 4, 4));
//    window.setVerticalSyncEnabled(true);

    /// Activate the window in the current thread
    window.setActive(true);

    renderer.init(scr_width_px, scr_height_px);

///// Initialize the game module
//    mechanics.init(world, dt);

    render_thread_loaded = true;

    float timeRenderLast = absClock.getElapsedTime().asSeconds();
    float timeRenderNew = timeRenderLast;
    float dtRender = 0.0;

    while (running)
    {
        /// Choose what to render
        culler.stage(scene, world); /// stage the scene from the world THIS breaks everything since non-shared shared pointers go out of scope

        /// calculate dtRender
        timeRenderNew = absClock.getElapsedTime().asSeconds();
        dtRender = timeRenderNew-timeRenderLast;
        timeRenderLast = timeRenderNew;

        /// draw
        renderer.draw(scene, dtRender);

        /// end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}

//void Master::testThreadTasks()
//{
//    while (running)
//    {
//        std::cout << "slowing down your program?\n";
//    }
//}

void Master::cleanUp()
{
    /// join threads
    render_thread.join();
}

bool Master::handleInput()
{
    /// Process fast input
    if (has_focus)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))      mechanics.playerMoveForward();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))      mechanics.playerMoveBackward();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))      mechanics.playerMoveLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))      mechanics.playerMoveRight();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))     mechanics.playerRotateUp();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   mechanics.playerRotateDown();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))   mechanics.playerRotateLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  mechanics.playerRotateRight();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))  mechanics.playerShift();
    }

    /// Slow event based input
    bool running = true;

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            running = false;
            break;
        case sf::Event::Resized:
            /// adjust the viewport when the window is resized
            renderer.resizeWindow(event.size.width, event.size.height);
            break;
        case sf::Event::LostFocus:
            has_focus = false;
            break;
        case sf::Event::GainedFocus:
            has_focus = true;
            break;
        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Right:
                {
                    window.setMouseCursorVisible (false);
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    press_pos_x = mouse_pos.x;
                    press_pos_y = mouse_pos.y;
                    rmb_down = true;
                } break;
            case sf::Mouse::Left:
                {
                    mechanics.playerAttack();
                } break;
            case sf::Mouse::Middle: break;
            default: break;
            } break;
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
            case sf::Mouse::Right:
                {
                    window.setMouseCursorVisible (true);            // SELF
                    rmb_down = false;
                } break;
            case sf::Mouse::Left: break;
            case sf::Mouse::Middle: break;
            default: break;
            } break;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                running = false;
                break;
            case sf::Keyboard::R:
                // do something
                break;
            case sf::Keyboard::E:
                mechanics.playerBlock();
                break;
            case sf::Keyboard::Q:
                mechanics.playerDodge();
                break;
            case sf::Keyboard::Space:
                mechanics.playerJump();
                break;
            case sf::Keyboard::F1:
                mechanics.func(1);
                break;
            case sf::Keyboard::F2:
                mechanics.func(2);
                break;
            case sf::Keyboard::F3:
                mechanics.func(3);
                break;
            case sf::Keyboard::F4:
                mechanics.func(4);
                break;
            case sf::Keyboard::F5:
                mechanics.func(5);
                break;
            case sf::Keyboard::F6:
                mechanics.func(6);
                break;
            case sf::Keyboard::F7:
                mechanics.func(7);
                break;
            case sf::Keyboard::F8:
                mechanics.func(8);
                break;
            case sf::Keyboard::F9:
                mechanics.func(9);
                break;
            case sf::Keyboard::F10:
                mechanics.func(10);
                break;
            case sf::Keyboard::F11:
                mechanics.func(11);
                break;
            case sf::Keyboard::F12:
                mechanics.func(12);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

    }

    if(rmb_down)
    {
//        if (camera_mode == FREE)
//        {
        //std::cout<<", ms";
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        int new_posx = mouse_pos.x;
        int new_posy = mouse_pos.y;
        int xoffset = new_posx - press_pos_x;
        int yoffset = new_posy - press_pos_y;
        //        cout<<" by "<<xoffset<<" and "<<yoffset;

        mechanics.playerRotateLeftVal(-xoffset/4.0);
        mechanics.playerRotateUpVal(-yoffset/4.0);
        mouse_pos = sf::Vector2i(press_pos_x, press_pos_y);
        sf::Mouse::setPosition(mouse_pos, window);
    }

    return running;
}
