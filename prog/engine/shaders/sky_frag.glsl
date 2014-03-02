# version 330 core

varying vec4 myvertex ;

uniform mat4 mv_mat;
uniform mat4 world_mat;

uniform vec4 sky_color;
uniform vec4 fog_color;

void main (void)
{
    const vec3 eyepos = vec3(0,0,0) ;
    vec4 _mypos = mv_mat * myvertex ;
    vec3 mypos = _mypos.xyz;

    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
