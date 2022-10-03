#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vertex_norm;

vec3 eyepos;
vec3 eye_normal;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor  = vec3(1.0f, 1.0f, 0.0f);

out vec3 FragPos;
out vec3 Normal;

void main(){

    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    FragPos = vec3(vModel*vec4(vVertex, 1.0));
    Normal = mat3(transpose(inverse(vModel))) * vertex_norm;
    
}
