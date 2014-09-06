#version 330 core


layout(location = 0) in vec4 InVertex;
layout(location = 2) in vec2 InTexCoord;

out vec2 texcoord;

// Values that stay constant for the whole mesh.
uniform sampler1D worldpos_tex;
uniform mat4 light_mvp;

uniform vec4 wind_params;

layout(std140) uniform GlobalUniforms
{
    mat4 proj_matUni;
    vec4 fog_colorUni;
    vec4 sky_colorUni;
    vec4 main_light_colorUni;
    vec3 main_light_dirUni;         // does this align?
    float zfarUni;
    vec4 wind_speed;
};

float wind_displace(float strength,
                    float stiffness,
                    float space_per,
                    float time_per,
                    float space_var,
                    float time_var,
                    float offset)
{
    float rel_str = strength/stiffness;
    return rel_str/2.0*(1+sin(2*3.1415*(1.0/space_per*space_var+1.0/time_per*time_var+offset)));
}

void main(){
    int instanceID = gl_InstanceID;

    texcoord = InTexCoord ;

    vec4 worldpos_sample = texelFetch(worldpos_tex, instanceID, 0);
    float theta_z = worldpos_sample.a;

    mat4 world_rotate = mat4(
           cos(theta_z),        0.0,            sin(theta_z),       0.0, // first column (not row!)
           0.0,                 1.0,            0.0,                0.0, // second column
           -sin(theta_z),       0.0,            cos(theta_z),       0.0,  // third column
           0.0,                 0.0,            0.0,                1.0 ); // assume only translation

	// wind shearing
    float time = wind_speed.w; // time stored in w component
	vec2 wind_displ = vec2(wind_displace(wind_speed.x, wind_params.x, wind_params.y, wind_params.z, worldpos_sample.x, time, 0.0),
                           wind_displace(wind_speed.z, wind_params.x, wind_params.y, wind_params.z, worldpos_sample.z, time, 1.3));



    mat4 wind_shear = mat4(
           1.0,                 0.0,            0.0,                0.0, // first column (not row!)
           wind_displ.x,        1.0,            wind_displ.y,       0.0, // second column
           0.0,                 0.0,            1.0,                0.0,  // third column
           0.0,                 0.0,            0.0,                1.0 ); // assume only translation


    mat4 world_translate = mat4(
           1.0,                 0.0,            0.0,                0.0, // first column (not row!)
           0.0,                 1.0,            0.0,                0.0, // second column
           0.0,                 0.0,            1.0,                0.0,  // third column
           worldpos_sample.x, worldpos_sample.y, worldpos_sample.z, 1.0 ); // assume only translation
//
//    mat4 world_pos_matrix = mat4(
//           cos(theta_z),        0.0,            sin(theta_z),       0.0, // first column (not row!)
//           0.0,                 1.0,            0.0,                0.0, // second column
//           -sin(theta_z),       0.0,            cos(theta_z),       0.0,  // third column
//           worldpos_sample.x, worldpos_sample.y, worldpos_sample.z, 1.0 ); // assume only translation


    mat4 world_pos_matrix =  world_translate * wind_shear * world_rotate;

    mat4 depthMVP = light_mvp * world_pos_matrix;

	gl_Position = depthMVP * InVertex;
}

