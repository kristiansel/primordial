#include "renderer.h"

Renderer::Renderer()
{
    scene = nullptr;
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::init(Scene &scene_in, unsigned int scr_width_in, unsigned int scr_height_in)
{
    /// set the scene (scene contains camera, actors, terrain etc)
    scene = &scene_in;

    cout << "Status: Using OpenGL " << glGetString(GL_VERSION) << "\n";

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /// Problem: glewInit failed, something is seriously wrong.
        cout << "Error: " << glewGetErrorString(err) << "\n";
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";

    /// Set clear values
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClearDepth(1.0);

    /// Enable depth testing
    glEnable(GL_DEPTH_TEST);

    /// Set up culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);

    /// Initiate perspective
    resizeWindow(scr_width_in, scr_height_in);

    /// Initialize shaders
    main_shader.load("shaders/simple_vert.glsl", "shaders/simple_frag.glsl");

    /// loading a mesh (TO BE REMOVED)
//    scene->meshes.push_back(Mesh());
//    scene->meshes.back().fromFile("assets_raw/models/sphere.obj");
//    scene->meshes.back().pos = vec3(0.0, 0.0, -2.0);
//
//    scene->meshes.push_back(Mesh());
//    scene->meshes.back().fromFile("assets_raw/models/quad.obj");
//    scene->meshes.back().pos = vec3(3.0, 0.0, -2.0);
}

void Renderer::draw()
{
    /// switch to main shader
    main_shader.switchTo();

    /// clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// prepare for perspective drawing
    mat4 mv = scene->camera.getModelViewMatrix();

    /// lights
    main_shader.setLights(mv);

    /// draw

//    for (auto it = scene->meshes.begin(); it!=scene->meshes.end(); it++)
//    {
//        main_shader.draw(*it, mv);
//    }

    for (auto it = scene->props.begin(); it!=scene->props.end(); it++)
    {
        main_shader.draw(*it, mv);
    }
}

void Renderer::resizeWindow(int w, int h, bool real)
{
    /*if (real)
    {
        s.width = w;
        s.height = h;
        // Rescale FBO and RBO as well
        render_stage.resize(w, h);
        blur1.resize(w/ratio, h/ratio);
        blur2.resize(w/ratio, h/ratio);

    }
    */
    mat4 prj ; // just like for lookat

    glMatrixMode(GL_PROJECTION);
    scene->camera.aspect = w / (float) h;
//        mv = Transform::perspective(fovy,aspect,nearz,farz) ;
    prj = Transform::perspective(scene->camera.fovy, scene->camera.aspect, scene->camera.nearz, scene->camera.farz) ;
    prj = glm::transpose(prj) ; // accounting for row major
    glLoadMatrixf(&prj[0][0]) ;

    glViewport(0, 0, w, h);

    /*
    //    post processing stuff
        pix_tex_coord_offset[0] = 1.0/(float)w;
        pix_tex_coord_offset[1] = 1.0/(float)h;
        updateKernel();
        */
}

