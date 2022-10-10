#version 330 core
layout (location = 1)in vec3 vNormal;

in vec3 vVertex;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor = vec3(1.0f, 0.5f, 1.0f);

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);
int phong = 32;

out vec3 fColor;

void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);

	//1.c - Gouraud diffuse light
	vec3 modelVertex = vec3(vModel*vec4(vVertex, 1.0)); // vertex in eye space
    vec3 modelNormal = mat3(transpose(inverse(vModel))) * vNormal; // normal in eye space

    vec3 norm = normalize(modelNormal);
    vec3 lightDir = normalize(light.position - modelVertex);
    float brightness = max(-1*dot(norm, lightDir) / (length(lightDir) * length(norm)), 0);
    vec3 diffuse = brightness*vColor*light.intensities*kd;

    //Ambient
    vec3 ambient = ka*vColor*light.intensities;

    // specular
    vec3 viewDir = normalize(-modelVertex);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), phong);
    vec3 specular = spec*ks*light.intensities*vColor;

    fColor = ambient+specular+diffuse; //Interpolate color

}
