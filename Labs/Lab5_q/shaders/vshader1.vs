#version 330
layout (location = 0)in vec3 vVertex;


vec3 eyepos;
vec3 eye_normal;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

vec3 lpos_world = vec3(0.0, 0.5, 1.0);

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.2, 0.2);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

float spec_exp = 4.0;


out vec4 fragColor;


void main(){

    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    
    eyepos = vec3(vModel * vec4(vVertex, 1.0));
    eye_normal = vec3( vModel * vec4(normalize(vVertex), 0.0));
    vec3 Ia = ka * La; 
    vec3 lpos_eye = vec3(vView * vec4(lpos_world, 1.0));
    vec3 dist_le = normalize(lpos_eye - eyepos);
    vec3 I_d = kd * max(dot(dist_le, normalize(eye_normal)), 0.0);

    fragColor = vec4(I_d + Ia, 1.0); 

    
}
