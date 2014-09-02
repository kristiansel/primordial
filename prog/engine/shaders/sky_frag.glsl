# version 330 core

varying vec4 myvertex ;

uniform mat4 mv_mat;
uniform mat4 world_mat;

//uniform vec4 sky_color;
//uniform vec4 fog_color;

//uniform float zfar;

layout(std140) uniform GlobalUniforms
{
    mat4 proj_matUni;
    vec4 fog_colorUni;
    vec4 sky_colorUni;
    vec4 main_light_colorUni;
    vec3 main_light_dirUni;         // does this align?
    float zfarUni;
};

void main (void)
{
    const vec3 eyepos = vec3(0,0,0) ;
    vec4 _mypos = mv_mat * myvertex ;
    vec3 mypos = _mypos.xyz;

    vec4 world_pos = world_mat * myvertex;

//    float fog_weight = 1.0-abs(world_pos.y)/60.0;
    float fog_weight = exp(-abs(world_pos.y)/90.0);
    fog_weight = clamp(fog_weight, 0, 1.0);

    //

    gl_FragColor = (1.0-fog_weight) * sky_colorUni + fog_weight * fog_colorUni;

    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
