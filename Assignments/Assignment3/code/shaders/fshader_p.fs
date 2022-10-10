#version 330 core

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

uniform vec3 vColor  = vec3(1.0f, 1.0f, 0.0f);

float spec_exp = 32.0;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

in vec3 FragPos;
in vec3 Normal;
in vec3 fColor;


out vec4 outColor;


void main(void) 
{   
    
	// Phong Shading
	//ambient 
	vec3 ambient = ka*light.intensities*fColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = kd*light.intensities*max(-dot(Normal, lightDir), 0.0f)*fColor;

    // reflect
    vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_exp);
    vec3 specular = ks*spec*light.intensities*fColor;

    outColor = vec4(ambient + diffuse + specular, 1.0);
}