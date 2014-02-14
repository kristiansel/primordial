#include "master.h"

Master::Master()
{
    /// initiate
    init();

    /// load resources
    loadResources();

    /// main loop
    mainLoop();

    /// cleanup
    cleanUp();
}

Master::~Master()
{
    /// dtor
}

void Master::init()
{
    /// internal initialization
    dt = 0.0;
    has_focus = true;

    /// to be loaded from settings
    scr_width_px = 1400;
    scr_height_px = 900;

    /// create the window and OpenGL context
    window.create(sf::VideoMode(scr_width_px, scr_height_px), "Primordial", sf::Style::Default, sf::ContextSettings(32, 0, 0, 4, 4));
    window.setVerticalSyncEnabled(true);

    renderer.init(scr_width_px, scr_height_px);

    /// Initialize the game module
    mechanics.init(world, dt);


}

void Master::loadResources()
{

}

void Master::mainLoop()
{
    /// run the main loop
    bool running = true;

    while (running)
    {
        /// start the watch
        clock.restart();

        /// handle user input
        running = handleInput();

        /// Step the simulation
        mechanics.step(world, dt);

        // cout << "dt = " << dt << "\n";

        /// Choose what to render
        culler.stage(scene, world); /// stage the scene from the world THIS breaks everything since non-shared shared pointers go out of scope

        /// draw...
        renderer.draw(scene);

        /// end the current frame (internally swaps the front and back buffers)
        window.display();

        /// record the time
        dt = clock.getElapsedTime().asSeconds();

//        cout << "frame time = " << dt << ", FPS = " << 1.0/dt << "\n";
//        system("PAUSE");
    }
}

void Master::cleanUp()
{

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
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                running = false;
                break;
            case sf::Keyboard::R:
                // do something
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

    return running;
}
