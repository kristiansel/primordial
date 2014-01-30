# version 120

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float[2] weights;

varying vec2 f_texcoord;

uniform float n = 0.5;

void main(void) {
    // multisample
//    vec4 NE = weights[0]*texture2D(texture0, f_texcoord+vec2(n/800.0, n/600.0)) + weights[1]*texture2D(texture1, f_texcoord+vec2(n/800.0, n/600.0));
//    vec4 NW = weights[0]*texture2D(texture0, f_texcoord+vec2(-n/800.0, n/600.0)) + weights[1]*texture2D(texture1, f_texcoord+vec2(-n/800.0, n/600.0));
//    vec4 SW = weights[0]*texture2D(texture0, f_texcoord+vec2(-n/800.0, -n/600.0)) + weights[1]*texture2D(texture1, f_texcoord+vec2(-n/800.0, -n/600.0));
//    vec4 SE = weights[0]*texture2D(texture0, f_texcoord+vec2(n/800.0, -n/600.0)) + weights[1]*texture2D(texture1, f_texcoord+vec2(n/800.0, -n/600.0));
//	gl_FragColor = (NE+NW+SW+SE)/4;
	gl_FragColor = weights[0]*texture2D(texture0, f_texcoord) + weights[1]*texture2D(texture1, f_texcoord); // no multisample
}
