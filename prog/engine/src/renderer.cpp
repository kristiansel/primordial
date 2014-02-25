#include "renderer.h"

Renderer::Renderer() : settings({0, 0})
{
    for (int i_cm = 0; i_cm<MAX_BONE_NUM; i_cm++)
    {
        clear_matrices[i_cm] = glm::mat4(1.0);
    }
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
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    /// Initialize Shadow mapping shader
    shadow_map.init();

    /// Initialize shaders
    main_shader.load("shaders/simple_vert.glsl", "shaders/simple_frag.glsl");

    /// Post processing init
    render_stage.init(scr_width_in, scr_height_in);
    blur1.init(scr_width_in/ratio, scr_height_in/ratio, "shaders/pp_pass2.frag.glsl");
    blur2.init(scr_width_in/ratio, scr_height_in/ratio, "shaders/pp_pass2.frag.glsl");
    comb1.init(scr_width_in, scr_height_in);

    /// Initiate perspective (this needs to be done after post processing init for now)
    resizeWindow(scr_width_in, scr_height_in);
}

void Renderer::draw(Scene &scene, float dt)
{
    /// First update all animations
    /// consider moving this out...
    for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
    {
        (*it)->updateAnim(dt);
    }

    /// This should look something like this: (for now light is hardcoded
    /// in the shadow_map.activate() method)
    //shadow_map.setLight(scene->getShadowLight())

    shadow_map.activate();

        glm::mat4 mv_fake(1.0);

        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
        {
            shadow_map.drawProp(*it, mv_fake);
        }

        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
        {
            shadow_map.drawActor(*it, mv_fake);
        }

    shadow_map.deactivate();

    resizeWindow(settings.width, settings.height, false);

    /// Set render "target" to draw to frame buffer
    render_stage.activate();

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

        /// Send default bone matrices
        glUniformMatrix4fv(main_shader.getBoneMat(), MAX_BONE_NUM, true, &clear_matrices[0][0][0]); // <-- THIS!

        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
        {
            main_shader.drawProp(*it, mv);
        }

        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
        {
//            (*it)->updateAnim(dt);
            main_shader.drawActor(*it, mv);
        }

    /// Finished main drawing, post processing
    render_stage.deactivate();

    ////     GENERATING POST-PROCESSING IMAGES
    resizeWindow(settings.width/ratio, settings.height/ratio, false);

    blur1.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);
    blur1.activateTextures(render_stage.fbo_texture, render_stage.fbo_depth);
    blur1.drawb();

    // removing this has no effect on performance
    blur2.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);
    blur2.activateTextures(blur1.fbo_texture, render_stage.fbo_depth);
    blur2.drawb();

    resizeWindow(settings.width, settings.height, false);

    // is absolutely killing performance
    comb1.activate(0.7, 0.5); // original
//    comb1.activate(0.7, 0.5);
    comb1.activateTextures(render_stage.fbo_texture, blur2.fbo_texture); // original
//    comb1.activateTextures(render_stage.fbo_texture, render_stage.fbo_depth);
    comb1.draw();
}

void Renderer::resizeWindow(int w, int h, bool real)
{
    if (real)
    {
        settings.width = w;
        settings.height = h;
        // Rescale FBO and RBO as well
        render_stage.resize(w, h);
        blur1.resize(w/ratio, h/ratio);
        blur2.resize(w/ratio, h/ratio);

    }

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


    //    post processing stuff
    pix_tex_coord_offset[0] = 1.0/(float)w;
    pix_tex_coord_offset[1] = 1.0/(float)h;
    updateKernel();
}

void Renderer::updateKernel()
{
    int edge = (KERNEL_SIZE-1)/2;
    for (int i=-edge; i<edge+1; i++)
    {
        for (int j=-edge; j<edge+1; j++)
        {
            kernelOffsety[(i+edge)+KERNEL_SIZE*(j+edge)] = pix_tex_coord_offset[0]*i;
            kernelOffsetx[(i+edge)+KERNEL_SIZE*(j+edge)] = pix_tex_coord_offset[1]*j;
        }
    }
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
