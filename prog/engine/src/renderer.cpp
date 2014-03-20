#include "renderer.h"

Renderer::Renderer() : settings({0, 0})
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
        // Problem: glewInit failed, something is seriously wrong.
        cerr << "Error: " << glewGetErrorString(err) << "\n";
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";

    // Set clear values
    //glClearColor(0.f/255.f, 80.f/255.f, 186.f/255.f, 0.0);

    glClearColor(0.f/255.f, 80.f/255.f, 186.f/255.f, 0.0);

    glClearDepth(1.0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up alpha blending
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable( GL_BLEND );

    // Set up culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Initialize Shadow mapping shader
    shadow_map.init();

    // Initialize shaders
    main_shader.init(shadow_map.getDepthTex());
    sky_shader.init();

    // Post processing init
    render_stage.init(scr_width_in, scr_height_in);
    blur1.init(scr_width_in/ratio, scr_height_in/ratio, "shaders/pp_pass2.frag.glsl");
    blur2.init(scr_width_in/ratio, scr_height_in/ratio, "shaders/pp_pass2.frag.glsl");
    comb1.init(scr_width_in, scr_height_in);

    // Initiate perspective (this needs to be done after post processing init for now)
    resizeWindow(scr_width_in, scr_height_in);

}

void Renderer::draw(Scene &scene, float dt)
{
//    // instead of passing dt... sample dt from the clock
//    dt = 1.f/60.f;

    // First update all animations
    // consider moving this out...
    // Or: Should update the time in animations somewhere else, then
    // in Renderer, only calculate the matrices of those in view.
    // Do this as doing blending etc
    for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
    {
        (*it)->updateAnim(dt);
        // The above is not thread safe if the actor is in the process of loading
    }

    // Set all things which are shared by shaders but can change in time
    glm::mat4 view_mat = scene.camera->getViewMatrix();
    glm::mat4 proj_mat = scene.camera->getProjectionMatrix();
//    setPerspective(settings.width, settings.height);

    // The above should be replaced by:
//    glm::mat4 vp_mat = scene.camera->getViewProjectionMatrix();

    const DirLight &mlight = (*scene.main_light);
    glm::mat4 mlight_vp = mlight.getVPmatrix();

    // This should look something like this: (for now light is hardcoded
    // in the shadow_map.activate() method)
    //shadow_map.setLight(scene->getShadowLight())
//#ifndef __unix

// something is thrown in here on linux...
// check shadowmap renderbuffer state etc...

//    shadow_map.activate();
   // shadow_map.activate(mlight_vp);
   shadow_map.activateDrawContent(mlight_vp);
        // Draw props (non-animated)
        shadow_map.clearBoneMatrices();

        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
        {
            shadow_map.drawProp(*it);
        }

        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
        {
            shadow_map.drawActor(*it);
        }

    shadow_map.deactivate();
//#endif
////
//    resizeWindow(settings.width, settings.height, false);
//
////    glm::vec4 fog_color = glm::vec4(1.0, 0.6, 0.8, 0.0);
//    glm::vec4 fog_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
//
//    // Set render "target" to draw to frame buffer
//    render_stage.activate();
//
//
//
//        // Set the values of the uniforms which are updated
//        // per-frame and switch to main shader
//        main_shader.activate(*(scene.camera),
//                             fog_color,
//                             mlight_vp,
//                             mlight/*, plights, num_plights, scene.fog_color*/);
//
//        // draw
//
//
//        // Draw props (non-animated)
//        main_shader.clearBoneMatrices();
//
//        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
//        {
//            // main_shader.drawProp(&**it);
//            main_shader.drawProp(*it);
//        }
//
//        // Draw actors;
//        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
//        {
//            // main_shader.drawActor(&**it);
//            main_shader.drawActor(*it);
//        }
//
//        // Draw "sky quad" following the camera
//        glm::vec4 sky_color = glm::vec4(0.f/255.f, 80.f/255.f, 186.f/255.f, 1.0);
//
//        // activate and draw in the same call
//        sky_shader.drawSkyQuad((*(scene.camera)), sky_color, fog_color);
//
//    // Finished main drawing, post processing
//    render_stage.deactivate();
//
//
//
//    // should be
////    blur1.activate(render_stage.fbo_texture, render_stage.fbo_depth);
////    blur1.drawb();
//
//    ///     GENERATING POST-PROCESSING IMAGES
//    resizeWindow(settings.width/ratio, settings.height/ratio, false);
//
//    // Consider managing the resizing in the post-proc-stage class
//    blur1.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);
//
//
//    blur1.activateTextures(render_stage.fbo_texture, render_stage.fbo_depth);
//    //std::cout << "REND gets here1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
//    blur1.drawb();
//    //std::cout << "REND gets here2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
//
//
//    // removing this has no effect on performance
//    blur2.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);
//    blur2.activateTextures(blur1.fbo_texture, render_stage.fbo_depth);
//    blur2.drawb();
//
//    resizeWindow(settings.width, settings.height, false);
//
//    // is absolutely killing performance
////    comb1.activate(1.000, 0.00); // original
//    comb1.activate(0.585, 0.415); // original
////    comb1.activate(0.7, 0.5);
//    comb1.activateTextures(render_stage.fbo_texture, blur2.fbo_texture); // original
////    comb1.activateTextures(render_stage.fbo_texture, render_stage.fbo_depth);
//
//
//
//    comb1.draw();

//
//    if (glGetError() != GL_NO_ERROR) // check error
//    {
//        std::cerr << "opengl error present\n";
//    }
    // Consider drawing lower resolution, and upscaling while applying
    // FXAA...
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

    // This inside each "stage"
    glViewport(0, 0, w, h);

    // Update this inside each "post-proc-stage"
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
