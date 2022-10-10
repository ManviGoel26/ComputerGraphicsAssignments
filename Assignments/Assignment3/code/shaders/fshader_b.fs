#version 330 core

// vec3 vColor = ve;

in vec3 FragPos;
in vec3 Normal;
in vec3 fColor;


out vec4 outColor;


void main(void) 
{  
    vec3 normalizedNormal = normalize(Normal);
    vec3 newColor = vec3((normalizedNormal.x+1)/2, (normalizedNormal.y+1)/2, (normalizedNormal.z + 1)/2)*fColor;
    outColor = vec4(newColor, 1.0);
}