#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vertex_norm;

vec3 eyepos;
vec3 eye_normal;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;

vec3 lpos_world = vec3(1.0, 1.0, 1.0);

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
    
    //to do 
    fragColor =vec4(vColor,1.0f);
	
    
}
