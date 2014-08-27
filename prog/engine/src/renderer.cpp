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
 //   glFrontFace(GL_CCW);
//    glCullFace(GL_BACK);
 //   glEnable(GL_CULL_FACE);

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

    // init the text shader
    text_shader.init();

    // Initiate perspective (this needs to be done after post processing init for now)
    resizeWindow(scr_width_in, scr_height_in);
}

void Renderer::draw(Scene &scene, float dt)
{
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

    const DirLight &mlight = (*scene.main_light);
    glm::vec3 cam_dir = scene.camera->getDir();
    glm::vec3 shadow_focus = scene.camera->pos + 12.f*glm::normalize(glm::vec3(cam_dir.x, 0.0, cam_dir.z));
    glm::mat4 mlight_vp = mlight.getVPmatrix(shadow_focus);

    glm::vec4 fog_color = glm::vec4(1.0, 1.0, 1.0, 1.0);

    // Why is this in here?
    glDisable(GL_BLEND);

    shadow_map.activate(mlight_vp);
    // shadow_map.activateDrawContent(mlight_vp);
        // Draw props (non-animated)
        shadow_map.clearBoneMatrices();

        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
        {
            shadow_map.drawProp(*it);
        }

        // Should preferably draw the terrain after actors and props
        std::vector<std::shared_ptr<Prop>>* terrain_patches = scene.terrain->getPatches();

//      // uncomment the below to make the terrain cast shadow
//        for (auto it = terrain_patches->begin(); it!=terrain_patches->end(); it++)
//        {
//            shadow_map.drawProp(*it);
//        }

        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
        {
            shadow_map.drawActor(*it);
        }

    shadow_map.deactivate();

    resizeWindow(settings.width, settings.height, false);


    // Set render "target" to draw to frame buffer
    render_stage.activate();

        // Set the values of the uniforms which are updated
        // per-frame and switch to main shader
        main_shader.activate(*(scene.camera),
                             fog_color,
                             mlight_vp,
                             mlight/*, plights, num_plights, scene.fog_color*/);

        // draw


        // Draw props (non-animated)
        main_shader.clearBoneMatrices();

        for (auto it = scene.props.begin(); it!=scene.props.end(); it++)
        {
            // main_shader.drawProp(&**it);
            main_shader.drawProp(*it);
        }

        for (auto it = terrain_patches->begin(); it!=terrain_patches->end(); it++)
        {
            main_shader.drawProp(*it);
            // why is this not showing...?
        }

        // Should preferably draw the terrain after actors and props
        // terrain_patches defined above


        // remove
        //std::terminate();

        // Draw actors;
        for (auto it = scene.actors.begin(); it!=scene.actors.end(); it++)
        {
            // main_shader.drawActor(&**it);
            main_shader.drawActor(*it);
        }

        //after this the bone matrices are soiled

        // Draw "sky quad" following the camera
        glm::vec4 sky_color = glm::vec4(0.f/255.f, 80.f/255.f, 186.f/255.f, 1.0);

        // activate and draw in the same call
        sky_shader.drawSkyQuad((*(scene.camera)), sky_color, fog_color);

    // Finished main drawing, post processing
    render_stage.deactivate();

    //     GENERATING POST-PROCESSING IMAGES
    resizeWindow(settings.width/ratio, settings.height/ratio, false);

    // Consider managing the resizing in the post-proc-stage class
    blur1.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);


    blur1.activateTextures(render_stage.fbo_texture, render_stage.fbo_depth);
    //std::cout << "REND gets here1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
    blur1.drawb();
    //std::cout << "REND gets here2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";


    // removing this has no effect on performance
    blur2.activate(KERNEL_SIZE, &kernelOffsetx[0], &kernelOffsety[0]);
    blur2.activateTextures(blur1.fbo_texture, render_stage.fbo_depth);
    blur2.drawb();

    resizeWindow(settings.width, settings.height, false);


    comb1.activate(0.585, 0.415); // original

    comb1.activateTextures(render_stage.fbo_texture, blur2.fbo_texture);
    comb1.draw();



    // Consider drawing lower resolution, and upscaling while applying
    // FXAA...
}

void Renderer::labelNumberPair(const char * label, float number, char * out)
{
    strcpy (out, label);
    char numstr[6];
    sprintf(numstr, "%f", number);
    strcat (out, numstr);
}

void Renderer::drawOverlay(InterfaceInfo interfaceInfo)
{
//    char str[80];
//    strcpy (str,"health: ");
//    char healthstr[4];
//    sprintf(healthstr, "%f", interfaceInfo.health);
//    strcat (str, healthstr);

    // bottom left corner: health/stats
    char health_disp[40];
    labelNumberPair("HP: ", interfaceInfo.health, health_disp);
    text_shader.printText2D(health_disp, 10, 10, 14);

    // Top left corner: frames per second
    char fps_disp[40];
    labelNumberPair("FPS: ", interfaceInfo.frames_per_second, fps_disp);
    text_shader.printText2D(fps_disp, 10, 580, 10);

    // Top right corner: instructions
    int line = 1;
    int right = 540;

    char move_disp[] = "Move: WASD";
    text_shader.printText2D(move_disp, right, 600-20*line++, 14);

    char swing_disp[] = "Swing: Left Mouse";
    text_shader.printText2D(swing_disp, right, 600-20*line++, 14);

    char dodge_disp[] = "Dodge: Q";
    text_shader.printText2D(dodge_disp, right, 600-20*line++, 14);

    char jump_disp[] = "Jump: SPACE";
    text_shader.printText2D(jump_disp, right, 600-20*line++, 14);

    char stance_disp[] = "Toggle Stance: TAB";
    text_shader.printText2D(stance_disp, right, 600-20*line++, 14);

    line++; // jump a line

    char rabbit_disp[] = "Add rabbit:     F1";
    text_shader.printText2D(rabbit_disp, right, 600-20*line++, 14);

    char sphere_disp[] = "Add sphere:     F2";
    text_shader.printText2D(sphere_disp, right, 600-20*line++, 14);

    char delete_disp[] = "Delete object:  F3";
    text_shader.printText2D(delete_disp, right, 600-20*line++, 14);

    char enemy_disp[] = "Spawn enem:     F4";
    text_shader.printText2D(enemy_disp, right, 600-20*line++, 14);

    char respawn_disp[] = "Player respawn: F5";
    text_shader.printText2D(respawn_disp, right, 600-20*line++, 14);

    char freecam_disp[] = "Toggle freecam: F6";
    text_shader.printText2D(freecam_disp, right, 600-20*line++, 14);
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
