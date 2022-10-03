#version 330 core

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.2, 0.2);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

uniform vec3 vColor  = vec3(1.0f, 1.0f, 0.0f);

vec3 Ia, Id, Is;
float spec_exp = 1.0;
vec3 lpos_world = vec3(1.0, 1.0, 1.0);

in vec3 FragPos;
in vec3 Normal;
in vec3 eyepos;
out vec4 fragColor;
void main()

{	
	// FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);

	// Phong Shading
	//ambient 
	Ia = ka*La;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lpos_world - FragPos);
    Id = kd*Ld*max(dot(Normal, lightDir), 0.0f);

    // reflect
    vec3 viewDir = normalize(eyepos - FragPos);
	// vec3 h = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_exp);
    // float spec = pow(max(dot(norm, h), 0.0), spec_exp);
    Is = ks*spec*Ls;

    fragColor = vec4(vColor*(Ia+Id+Is), 1.0f);
	// FragColor = fragColor;
}
