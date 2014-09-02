# version 330 core

varying vec4 myvertex ;

attribute vec4 vertex ;

uniform mat4 mv_mat;

layout(std140) uniform GlobalUniforms
{
    mat4 proj_matUni;
    vec4 fog_colorUni;
    vec4 sky_colorUni;
    vec4 main_light_colorUni;
    vec3 main_light_dirUni;         // does this align?
    float zfarUni;
};

void main() {
    myvertex = vertex ;

    gl_Position = mv_mat * myvertex ;
}

