#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vertex_norm;

vec3 eyepos;
vec3 eye_normal;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor  = vec3(1.0f, 1.0f, 0.0f);

vec3 lpos_world = vec3(1.0, 1.0, 1.0);

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.2, 0.2);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);
vec3 Ia, Id, Is;
float spec_exp = 1.0;


out vec4 fragColor;


void main(){

    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);


    // gouraud shading
    vec3 modelVertex = vec3(vModel*vec4(vVertex, 1.0)); // vertex in eye space
    vec3 modelNormal = mat3(transpose(inverse(vModel))) * vertex_norm; // normal in eye space
    
    // ambient
    Ia = ka*La;

    // diffuse
    vec3 norm = normalize(vertex_norm);
    vec3 lightDir = normalize(lpos_world - modelVertex);
    Id = kd*Ld*max(dot(modelNormal, lightDir), 0.0f);

    // reflect
    vec3 viewDir = normalize(eyepos - modelVertex);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_exp);
    Is = ks*spec*Ls;
    
    fragColor = vec4(vColor*(Ia+Id+Is), 1.0f);
	
    
}
