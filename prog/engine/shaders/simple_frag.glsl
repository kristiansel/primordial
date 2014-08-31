# version 330 core

varying vec3 mynormal ;
varying vec4 myvertex ;
varying vec2 mytexco ;

varying vec4 shadowvertex ;

//uniform int num_lights ; // are we lighting.
//
//uniform vec4 light_posns[10];
//uniform vec4 light_cols[10];

uniform vec3 main_light_dir;
uniform vec4 main_light_color;

// With texture
uniform sampler2D tex; // the active texture
uniform sampler2DShadow shadow_depth; // the shadow mapping depth texture

uniform mat4 mv_mat;
uniform mat4 to_world_space_mat;

uniform vec4 fog_color;
uniform vec4 sky_color;
uniform float zfar;

uniform mat4 shadowmap_mvp_mat;

uniform vec4 ambient ;
uniform vec4 diffuse ;
uniform vec4 specular ;
uniform float shininess ;
uniform vec4 emission ;

uniform vec2 poisson_disk[16] = vec2[](
   vec2( -0.81544232, -0.87912464 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( -0.94201624, -0.39906216 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

uniform vec2 foursamples[4] = vec2[](
    vec2(-0.50000, -0.50000),
    vec2(-0.50000, 0.50000),
    vec2(0.50000, -0.50000),
    vec2(0.50000, 0.50000)
);

uniform int NUM_SHADOW_SAMPLES = 4;

uniform float SHADOW_STRENGTH = 0.87;

// How can I make this more const...?
//float SHADOW_DECR = SHADOW_STRENGTH/NUM_SHADOW_SAMPLES;
float SHADOW_DECR = 0.2175; // = 0.87/4
float SHADOW_DECR16 = 0.054375; // = 0.87/16

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main (void)
{
    // They eye is always at (0,0,0) looking down -z axis
    // Also compute current fragment position and direction to eye

    const vec3 eyepos = vec3(0,0,0) ;
    vec4 _mypos = mv_mat * myvertex ;
    vec3 mypos = _mypos.xyz; //_mypos.xyz / _mypos.w ; // Dehomogenize current location (what is this shit?)
    vec3 eyedirn = normalize(eyepos - mypos) ;

    // Compute normal, needed for shading.
    // Simpler is vec3 normal = normalize(gl_NormalMatrix * mynormal) ;
//        vec3 _normal = (gl_ModelViewMatrixInverseTranspose*vec4(mynormal,1.0)).xyz ;
    // vec3 _normal = (nrm_mat * vec4(mynormal, 0.0)).xyz;
    vec3 _normal = (mv_mat * vec4(mynormal, 0.0)).xyz;

    vec3 normal = normalize(_normal) ;


    vec4 SUM = vec4(0,0,0,0) ;
//        for (int i = 0; i < num_lights; i ++)   //Need to increase the i<1
//        {
//    vec4 light_cols_att;
//    if (light_posns[i].w > 0.001 || light_posns[i].w < -0.001)
//    {
///                vec3 position = light_posns[i].xyz / light_posns[i].w; // dehomogenize light positio
//        vec3 position = light_posns[i].xyz;
//        light_cols_att = light_cols[i]*1/(pow(length(mypos - position),1)); // attenuate
///                light_cols_att = vec4(1.0, 1.0, 1.0, 1.0);
//        //light_cols_att = light_cols[i];
//        // float light_dist = ((light_posns[0].x - mypos.x) * (light_posns[0].x - mypos.x) + (light_posns[0].y  - mypos.y)* (light_posns[0].y  - mypos.y)+ (light_posns[0].z  - mypos.z)* (light_posns[0].z - mypos.z));
//        // light_cols_att = vec4(light_cols[i].r/light_dist, light_cols[i].g/light_dist, light_cols[i].b/light_dist, 1.0);
//        lightdirection = normalize (position - mypos);
//    }
//    else
//    {
    vec3 lightdirection = normalize(main_light_dir);
    vec4 light_cols_att = main_light_color;
//                light_cols_att = vec4(0.0, 0.0, 0.0, 1.0);
//    }
    vec3 halfangle = normalize (lightdirection + eyedirn); // compute half-angle

    float nDotL = dot(normal, lightdirection);
    float nDotH = dot(normal, halfangle);

    // Implementing the homework lighting equation
    vec4 term = light_cols_att * (diffuse * max(nDotL, 0) + specular * pow(max(nDotH, 0), shininess)) ;
    SUM = SUM + term;
//        }

    // This should be changable
    vec4 amb = vec4(0.05, 0.05, 0.05, 1.0);

    // Shadow Mapping
    float bias = 0.0025; // cosTheta is dot( n,l ), clamped between 0 and 1
//    float bias = 0.001*tan(acos(nDotL));
//    bias = clamp(bias, 0, 0.1);


//    float visibility = 1.0;
//    for (int i=0;i<NUM_SHADOW_SAMPLES;i++){
//        int index = int(16.0*random(floor(myvertex.xyz*1000.0), i))%16;
//
//        float vis_sample = texture( shadow_depth, vec3(shadowvertex.xy + poisson_disk[index]/700.0,   (shadowvertex.z-bias) ) );
//        visibility -= (1.0-vis_sample)*SHADOW_DECR;
//    }

    // 16 sample shadowmap... wow. Tested early jump out of loop
    // did not work well. Consider smaller shadowmap and follow camera
    // plus 8 samples
    float visibility = 1.0;
    for (int i=0;i<16;i++){
        float vis_sample = texture( shadow_depth, vec3(shadowvertex.xy + poisson_disk[i]/700.0,   (shadowvertex.z-bias) ) );
        visibility -= (1.0-vis_sample)*SHADOW_DECR16;
    }

//    float visibility = texture( shadow_depth, vec3(shadowvertex.xy, shadowvertex.z-bias) );

//    float depthsample = texture( shadow_depth, vec3(shadowvertex.xy,  (shadowvertex.z-bias)/shadowvertex.w) );
//    float visibility = (depthsample  <  shadowvertex.z) ? 0.0 : 1.0;
//    float visibility = (depthsample  <  (shadowvertex.z-bias)) ? 0.0 : 1.0;

    vec4 color = amb + emission + visibility * SUM ;
    vec4 texel = texture(tex, mytexco.st);
    vec4 local_color = vec4(texel.rgb * color.rgb, texel.a);

    if (local_color.a < 0.5)
    {
        discard;
    }
    else
    {
        vec4 world_pos = to_world_space_mat * myvertex;

        float distance_fallof = exp(-3.0*(mypos.z/zfar+1.0));
        distance_fallof = (distance_fallof-exp(-3.0))/(1.0-exp(-3.0));
        float height_fallof = exp(-abs(world_pos.y)/90.0);

        float fog_weight = height_fallof * distance_fallof  ;

        fog_weight = clamp(fog_weight, 0, 1.0);

        // Make close distance fog bluer than far away fog
        // This looks truly beautiful. Should probably pass the blue
        // as the same color as the sky
        vec4 final_fog_color = (1.0-distance_fallof)*sky_color + distance_fallof * fog_color;

        gl_FragColor = (1.0-fog_weight) * local_color + fog_weight * final_fog_color;
    }
}
