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
    dt = -1.0;

    /// create the window and OpenGL context
    window.create(sf::VideoMode(1920, 1080), "Primordial", sf::Style::Default, sf::ContextSettings(32, 0, 0, 4, 4));
    window.setVerticalSyncEnabled(true);
    cout << "Status: Using OpenGL " << glGetString(GL_VERSION) << "\n";

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /// Problem: glewInit failed, something is seriously wrong.
      cout << "Error: " << glewGetErrorString(err) << "\n";
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";

    /// Initialize the renderer module
    renderer.init();

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

        /// draw...
        renderer.draw();

        /// end the current frame (internally swaps the front and back buffers)
        window.display();

        /// record the time
        dt = clock.getElapsedTime().asSeconds();
    }
}

void Master::cleanUp()
{

}

bool Master::handleInput()
{
    bool running = true;

    /// handle events
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            /// end the program
            running = false;
        }
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
        {
            running = false;
        }
        if (event.type == sf::Event::Resized)
        {
            /// adjust the viewport when the window is resized
            resizeWindow(event.size.width, event.size.height);
        }

    }

    return running;
}

void Master::resizeWindow(int w, int h, bool real)
{
    /// move everything that has to do with rendering to the
    /// renderer class
    /*if (real)
    {
        s.width = w;
        s.height = h;
        // Rescale FBO and RBO as well
        render_stage.resize(w, h);
        blur1.resize(w/ratio, h/ratio);
        blur2.resize(w/ratio, h/ratio);

    }

    mat4 prj ; // just like for lookat

    glMatrixMode(GL_PROJECTION);
    cam.aspect = w / (float) h;
//        mv = Transform::perspective(fovy,aspect,nearz,farz) ;
    prj = Transform::perspective(cam.fovy,cam.aspect,cam.nearz,cam.farz) ;
    prj = glm::transpose(prj) ; // accounting for row major
    glLoadMatrixf(&prj[0][0]) ;
    */
    glViewport(0, 0, w, h);

/*
//    post processing stuff
    pix_tex_coord_offset[0] = 1.0/(float)w;
    pix_tex_coord_offset[1] = 1.0/(float)h;
    updateKernel();
    */
}
