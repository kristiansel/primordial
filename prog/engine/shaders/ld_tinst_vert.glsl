# version 330 core

varying vec3 mytexco ;
varying vec4 shadowvertex ;
varying vec3 mypos ; // camera space
varying vec3 normal ;  // camera space
varying vec4 world_pos ; // world space

uniform sampler1D worldpos_tex;
uniform mat4 cam_matrix;
uniform mat4 light_bias_mat;

//
layout(location = 0) in vec4 InVertex;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InTexCoord;

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
    int instanceID = gl_InstanceID;

	mytexco = InTexCoord ;

	vec4 worldpos_sample = texelFetch(worldpos_tex, instanceID, 0);

	//vec4 worldpos_sample = vec4(instanceID*3.0, 0.0, 0.0, 1.0);

    mat4 world_pos_matrix = mat4(
           1.0,                 0.0,            0.0,                0.0, // first column (not row!)
           0.0,                 1.0,            0.0,                0.0, // second column
           0.0,                 0.0,            1.0,                0.0,  // third column
           worldpos_sample.x, worldpos_sample.y, worldpos_sample.z, 1.0 ); // assume only translation

	mat4 mv_matrix_calc = cam_matrix * world_pos_matrix;
	mat4 shadow_matrix = light_bias_mat * world_pos_matrix;

    world_pos = world_pos_matrix * InVertex;
    vec4 mypos4 = mv_matrix_calc * InVertex;
    mypos = (mypos4).xyz;
    shadowvertex = shadow_matrix * world_pos_matrix * InVertex;
    normal = normalize( (mv_matrix_calc * vec4(InNormal, 0.0) ).xyz) ;


//    mat4 my_mat = mat4(
//           1.0,                 0.0,            0.0,                0.0, // first column (not row!)
//           0.0,                 1.0,            0.0,                0.0, // second column
//           0.0,                 0.0,            1.0,                0.0,  // third column
//           0.0,                 0.0,           -5.0,                1.0 ); // assume only translation

    gl_Position = proj_matUni * mypos4;

    //mat4 my_fixed_mat = world_pos_matrix * my_mat;

    //gl_Position = proj_matUni * my_fixed_mat * InVertex;
}

