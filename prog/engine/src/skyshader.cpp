#include "skyshader.h"

SkyShader::SkyShader() :
    sky_quad(nullptr), vbo_fbo_vertices(0)
{
    //ctor
}


void SkyShader::init()
{
    // get program id from shader text files
    ShaderBase::load("shaders/sky_vert.glsl", "shaders/sky_frag.glsl");

    // bind uniforms
    uniforms.mvp_matrix = glGetUniformLocation(getProgramID(), "mv_mat");
    uniforms.world_matrix = glGetUniformLocation(getProgramID(), "world_mat");

    uniforms.sky_color = glGetUniformLocation(getProgramID(), "sky_color");
    uniforms.fog_color = glGetUniformLocation(getProgramID(), "fog_color");

    uniforms.zfar = glGetUniformLocation(getProgramID(), "zfar");

    { // test
        GLfloat fbo_vertices[] =
        {
            -1, -1, 0.0, 1.0,
            1, -1, 0.0, 1.0,
            -1,  1, 0.0, 1.0,
            1,  1, 0.0, 1.0
        };
        glGenBuffers(1, &vbo_fbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // get/enable attribs/uniforms and everything
        char* attribute_name = "vertex";
        attributes.vertex = glGetAttribLocation(getProgramID(), attribute_name);
        if (attributes.vertex == -1)
        {
            fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    //        return 0;
        }

        int isValid;
        glValidateProgram(getProgramID());
        glGetProgramiv(getProgramID(), GL_VALIDATE_STATUS, &isValid);

        glEnableVertexAttribArray(attributes.vertex);

    }


//    { // OLD
//    // bind attributes
//        attributes.vertex = glGetAttribLocation(getProgramID(), "vertex") ;
//
//        glEnableVertexAttribArray(attributes.vertex);
//
//        // procedurally create the quad
//        int num_verts = 4;
//        int num_tris = 2;
//        Vertex* verts = new Vertex [num_verts];
//        Triangle* tris = new Triangle [num_tris];
//
//        //     1-------2
//        //     | _0___/|
//        //     |/   1  |
//        //     0-------3
//
//        verts[0].position = glm::vec4(-1.0, -1.0, 0.0, 1.0);
//        verts[1].position = glm::vec4(-1.0, 1.0, 0.0, 1.0);
//        verts[2].position = glm::vec4(1.0, 1.0, 0.0, 1.0);
//        verts[3].position = glm::vec4(1.0, -1.0, 0.0, 1.0);
//
//        tris[0].indices[0] = 0;
//        tris[0].indices[1] = 2;
//        tris[0].indices[2] = 1;
//
//        tris[1].indices[0] = 0;
//        tris[1].indices[1] = 3;
//        tris[1].indices[2] = 2;
//
//        // assing the quad to the mesh;
//        sky_quad = new Mesh;
//        sky_quad->fromMemory(verts, num_verts, tris, num_tris);
//    //    sky_quad->fromMemory(verts, 4, tris, 2);
//
//        delete [] verts;
//        delete [] tris;
//    }
}

void SkyShader::drawSkyQuad(const Camera &cam_in,
                            const glm::vec4 &sky_color_in,
                            const glm::vec4 &fog_color_in)
{
    // switch to
    ShaderBase::switchTo();

    // update quad position and scale based on vp_mat_in
    // could take the camera as argument as well for ease of
    // calculation
    glm::mat4 proj_mat = cam_in.getProjectionMatrix();

    glm::mat4 t = glm::translate( glm::mat4(1.0),
                                  glm::vec3(0.0, 0.0, -(cam_in.farz-0.5)) );

    glm::mat4 s = glm::scale( glm::mat4(1.0),
                              glm::vec3(cam_in.aspect*cam_in.farz/2.0, cam_in.farz/2.0, 1.0) );

    glm::mat4 mvp_mat = proj_mat * t * s;
    glUniformMatrix4fv(uniforms.mvp_matrix, 1, false, &mvp_mat[0][0]);

    glm::mat4 world_mat = cam_in.getTransformMatrix() * t * s;
    glUniformMatrix4fv(uniforms.world_matrix, 1, false, &world_mat[0][0]);

    glUniform4fv(uniforms.sky_color, 1, &sky_color_in[0]);
    glUniform4fv(uniforms.fog_color, 1, &fog_color_in[0]);

    glUniform1f(uniforms.zfar, cam_in.farz);

//    std::cout << "farz = " << cam_in.farz << "\n";
//    t = glm::translate( glm::mat4(1.0),
//                                  glm::vec3(0.0, 0.0, -599.0) );
//
//    s = glm::scale( glm::mat4(1.0),
//                              glm::vec3(100.0, 100.0, 100.0) );
//
//    mvp_mat = proj_mat * t * s;
    glUniformMatrix4fv(uniforms.mvp_matrix, 1, false, &mvp_mat[0][0]);


    // Checklist
    // VBO id and IBO id same as at generation time
    // transform matrix
    // zfar and color
    // state of shader...?


//    std::cout << "proj_mat: \n" << proj_mat << "\n\n";
//
//    std::terminate();
//    std::cout << "t: \n" << t << "\n\n";
//    std::cout << "s: \n" << s << "\n\n";
//    std::cout << "mvp_mat: \n" << mvp_mat << "\n\n";
//    std::cout << "cam_in.aspect: \n" << cam_in.aspect << "\n\n";
//    std::cout << "cam_in.farz: \n" << cam_in.farz << "\n\n";
//
//    std::cout << "vbo_id: \n" << sky_quad->getVBOid() << "\n\n";
//    std::cout << "ibo_id: \n" << sky_quad->getIBOid() << "\n\n";
//
//    std::cout << "sky_quad->getTriNum(): \n" << sky_quad->getTriNum() << "\n\n";
//
//    std::cout << "glGetError(): \n" << ((glGetError()!=GL_NO_ERROR) ? "error" : "no error") << "\n\n";
//
//    std::cout << "terminating prematurely\n";
//    std::terminate();

    //if (glGetError() != GL_NO_ERROR) std::cout << "something wrong\n";
    // Draw call
    // Sky is not drawn on linux period...



//    { // OLD
//    // Bind vertex data
//        glBindBuffer(GL_ARRAY_BUFFER, sky_quad->getVBOid());
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_quad->getIBOid());
//
//        // Apparently, the below is buffer specific? It needs to be here at least. Look into VAO
//        // Or separate buffers for each attribute (corresponds better to the .obj 3d format)
//        glVertexAttribPointer(attributes.vertex,       4, GL_FLOAT,    GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)                      );
//
//        /// Why does it have to wait for main thread not to crash here...
//    //    std::cout << "sky_quad->getTriNum() : " <<  sky_quad->getTriNum() << std::endl;
//    //
//        glDrawElements(GL_TRIANGLES, 3*sky_quad->getTriNum(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
//
//    //    std::cout << "gets here\n";
//
//        // Not sure if this is necessary unless other code is badly written
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    }
//
    { // NEW
        // Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
        glVertexAttribPointer(
            attributes.vertex,  // attribute
            4,                  // number of elements per vertex, here (x,y)
            GL_FLOAT,           // the type of each element
            GL_FALSE,           // take our values as-is
            0,                  // no extra data between each position
            0                   // offset of first element
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //std::cout << "drawing arrays\n";
    }
}

SkyShader::~SkyShader()
{
    std::cout << "deleting skyshader\n";
    delete sky_quad;
    //dtor
}
