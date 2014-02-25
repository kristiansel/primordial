# version 120


// varying vec4 color ;
varying vec3 mynormal ;
varying vec4 myvertex ;
varying vec2 mytexco ;
//varying vec3 ReflectDir ;

uniform int num_lights ; // are we lighting.

uniform vec4 light_posns[10];
uniform vec4 light_cols[10];

uniform sampler2D tex; // refers to something
uniform int hasTexture; // 1
//uniform int cubeMapped;
//uniform mat4 invCamRotMat;

uniform mat4 nrm_mat;
uniform mat4 mv_mat;

// Now, set the material parameters.  These could be varying and/or bound to
// a buffer.  But for now, I'll just make them uniform.
// I use ambient, diffuse, specular, shininess as in OpenGL.
// But, the ambient is just additive and doesn't multiply the lights.

uniform vec4 ambient ;
uniform vec4 diffuse ;
uniform vec4 specular ;
uniform float shininess ;
uniform vec4 emission ;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess)
{

    float nDotL = dot(normal, direction)  ;
    vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;

    float nDotH = dot(normal, halfvec) ;
    vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ;

    vec4 retval = lambert + phong ;
    return retval ;
}

vec3 lightdirection;

void main (void)
{
    if (num_lights == 0) gl_FragColor = vec4(0.8, 0.8, 0.8, 1.0);
    else
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
        vec3 _normal = (nrm_mat * vec4(mynormal, 0.0)).xyz;

        vec3 normal = normalize(_normal) ;


        vec4 SUM = vec4(0,0,0,0) ;
        for (int i = 0; i < num_lights; i ++)   //Need to increase the i<1
        {
            vec4 light_cols_att;
            if (light_posns[i].w > 0.001 || light_posns[i].w < -0.001)
            {
//                vec3 position = light_posns[i].xyz / light_posns[i].w; // dehomogenize light positio
                vec3 position = light_posns[i].xyz;
                light_cols_att = light_cols[i]*1/(pow(length(mypos - position),1)); // attenuate
//                light_cols_att = vec4(1.0, 1.0, 1.0, 1.0);
                //light_cols_att = light_cols[i];     /// NOT ATTENUATED
                float light_dist = ((light_posns[0].x - mypos.x) * (light_posns[0].x - mypos.x) + (light_posns[0].y  - mypos.y)* (light_posns[0].y  - mypos.y)+ (light_posns[0].z  - mypos.z)* (light_posns[0].z - mypos.z));
                light_cols_att = vec4(light_cols[i].r/light_dist, light_cols[i].g/light_dist, light_cols[i].b/light_dist, 1.0);
                lightdirection = normalize (position - mypos);
            }
            else
            {
                lightdirection = normalize(light_posns[i].xyz);
                light_cols_att = light_cols[i];
//                light_cols_att = vec4(0.0, 0.0, 0.0, 1.0);
            }
            vec3 halfangle = normalize (lightdirection + eyedirn); // compute half-angle

            float nDotL = dot(normal, lightdirection);
            float nDotH = dot(normal, halfangle);

            // Implementing the homework lighting equation
            vec4 term = light_cols_att * (diffuse * max(nDotL, 0) + specular * pow(max(nDotH, 0), shininess)) ;
            SUM = SUM + term;
        }

        //vec4 ct = texture2D(tex, mytexco);
        if (hasTexture==0)
        {
//            vec4 amb = ambient;
            vec4 amb = vec4(0.05, 0.05, 0.05, 1.0);
            gl_FragColor = amb + emission + SUM ;//+ ct; // gl_FragColor = ambient + emmisive + SUM ;
        }
        else
        {
//            vec4 amb = ambient;
            vec4 amb = vec4(0.05, 0.05, 0.05, 1.0);
            vec4 color = amb + emission + SUM ;
            vec4 texel = texture2D(tex, mytexco.st);  // this is somehow undefined???
            gl_FragColor = vec4(texel.rgb * color.rgb, 1);
//            gl_FragColor = vec4(mytexco.s, mytexco.t, 0.5, 1);
        }
//        float light_dist = sqrt((light_posns[0].x - mypos.x) * (light_posns[0].x - mypos.x) + (light_posns[0].y  - mypos.y)* (light_posns[0].y  - mypos.y)+ (light_posns[0].z  - mypos.z)* (light_posns[0].z - mypos.z));
//        gl_FragColor = vec4(1/light_dist, 1/light_dist, 1/light_dist, 1.0);
//        gl_FragColor = vec4(normal.x, normal.y, normal.z, 1.0);
//        gl_FragColor = vec4(lightdirection.x, lightdirection.y, lightdirection.z, 1.0);

    }
}
