#include "renderer.h"

Renderer::Renderer()
{
//    scene = nullptr;
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::init(unsigned int scr_width_in, unsigned int scr_height_in)
{
    cout << "Status: Using OpenGL " << glGetString(GL_VERSION) << "\n";

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /// Problem: glewInit failed, something is seriously wrong.
        cerr << "Error: " << glewGetErrorString(err) << "\n";
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";

    /// Set clear values
    glClearColor(0.0, 0.0, 0.0, 0.0);
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
//    scene->meshes.back().pos = glm::vec3(0.0, 0.0, -2.0);
//
//    scene->meshes.push_back(Mesh());
//    scene->meshes.back().fromFile("assets_raw/models/quad.obj");
//    scene->meshes.back().pos = glm::vec3(3.0, 0.0, -2.0);
}

void Renderer::draw(Scene &scene, float dt)
{
    /// switch to main shader
    main_shader.switchTo();

    /// clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// prepare for perspective drawing
    glm::mat4 mv = scene.camera->getModelViewMatrix();
    // glm::mat4 mv = getModelViewMatrix(scene.camera);

    /// lights
    main_shader.setLights(mv);

    /// draw

//    for (auto it = scene->meshes.begin(); it!=scene->meshes.end(); it++)
//    {
//        main_shader.draw(*it, mv);
//    }

    /// Send default bone
    glm::mat4 clearMatrix = glm::mat4(1.0);
    glUniformMatrix4fv(main_shader.getBoneMat(), 1, true, &clearMatrix[0][0]); // <-- THIS!

    for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
    {
        main_shader.drawProp(*it, mv);
    }

    for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
    {
        (*it)->updateAnim(dt);
        main_shader.drawActor(*it, mv);
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
    glm::mat4 prj ; // just like for lookat


    /// 2 issues:
    /// 1) Should really resizing change the perspective?
    /// 2) This is the last bit of fixed functionality used
    ///    Definitly need to revamp this
    glMatrixMode(GL_PROJECTION);

    perspective.aspect = w / (float) h;

    prj = glm::perspective(3.14159265f*perspective.fovy/180.f, perspective.aspect, perspective.nearz, perspective.farz);
    glLoadMatrixf(&prj[0][0]) ;

    glViewport(0, 0, w, h);

    /*
    //    post processing stuff
        pix_tex_coord_offset[0] = 1.0/(float)w;
        pix_tex_coord_offset[1] = 1.0/(float)h;
        updateKernel();
        */
}

Renderer::Perspective::Perspective()
{
    fovy = 40; // field of vertical view
    aspect = 1; // Ahhh! Where is this used?
    nearz = 0.1; // near z clipping plane
    farz = 200; // far z clipping plane
}

Renderer::Perspective::~Perspective()
{

}

Renderer::Perspective::Perspective(float fovy, float aspect, float nearz, float farz)
{
    setPerspective(fovy, aspect, nearz, farz);
}

void Renderer::Perspective::setPerspective(float fovy_in, float aspect_in, float nearz_in, float farz_in)
{
    fovy = fovy_in;
    aspect = aspect_in;
    nearz = nearz_in;
    farz = farz_in;
}

//glm::mat4 Renderer::Perspective::getModelViewMatrix()
//{
//    glm::mat4 mv = Transform::lookAt(pos, dir, up) ;
//    mv = glm::transpose(mv) ;
//    return mv;
//}
