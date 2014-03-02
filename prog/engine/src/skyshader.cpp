#include "skyshader.h"

SkyShader::SkyShader() :
    sky_quad(nullptr)
{
    //ctor
}


void SkyShader::init()
{
    /// get program id from shader text files
    ShaderBase::load("shaders/sky_vert.glsl", "shaders/sky_frag.glsl");

    /// bind uniforms
    uniforms.mvp_matrix = glGetUniformLocation(getProgramID(), "mv_mat");
    uniforms.world_matrix = glGetUniformLocation(getProgramID(), "world_mat");

    uniforms.sky_color = glGetUniformLocation(getProgramID(), "sky_color");
    uniforms.fog_color = glGetUniformLocation(getProgramID(), "fog_color");

    /// bind attributes
    attributes.vertex = glGetAttribLocation(getProgramID(), "vertex") ;

    /// procedurally create the quad
    int num_verts = 4;
    int num_tris = 2;
    Vertex* verts = new Vertex [num_verts];
    Triangle* tris = new Triangle [num_tris];

    ///     1-------2
    ///     | _0___/|
    ///     |/   1  |
    ///     0-------3

    verts[0].position = glm::vec4(-1.0, -1.0, 0.0, 1.0);
    verts[1].position = glm::vec4(-1.0, 1.0, 0.0, 1.0);
    verts[2].position = glm::vec4(1.0, 1.0, 0.0, 1.0);
    verts[3].position = glm::vec4(1.0, -1.0, 0.0, 1.0);

    tris[0].indices[0] = 0;
    tris[0].indices[1] = 2;
    tris[0].indices[2] = 1;

    tris[1].indices[0] = 0;
    tris[1].indices[1] = 3;
    tris[1].indices[2] = 2;

    /// assing the quad to the mesh;
    sky_quad = new Mesh;
    sky_quad->fromMemory(verts, num_verts, tris, num_tris);
//    sky_quad->fromMemory(verts, 4, tris, 2);

    delete [] verts;
    delete [] tris;
}

void SkyShader::drawSkyQuad(const Camera &cam_in,
                            const glm::vec4 &sky_color_in,
                            const glm::vec4 &fog_color_in)
{
    /// switch to
    ShaderBase::switchTo();

    /// update quad position and scale based on vp_mat_in
    /// could take the camera as argument as well for ease of
    /// calculation
    glm::mat4 mvp_mat = glm::mat4(1.0);

//    glm::mat4 t = glm::translate(glm::mat4(1.0), cam_in.)

    /// send world matrix
    /// mvp matrix
}

SkyShader::~SkyShader()
{
    delete sky_quad;
    //dtor
}
