#version 330 core
layout (location = 5)in vec3 vNormal;

in vec3 vVertex;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor = vec3(1.0f, 1.0f, 1.0f);

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);
int phong = 32;

out vec3 FragPos;
out vec3 Normal;
out vec3 fColor;


void main() 
{
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    fColor = vColor;
    FragPos = vec3(vModel*vec4(vVertex, 1.0));
    Normal = mat3(transpose(inverse(vModel))) * vNormal;
}
