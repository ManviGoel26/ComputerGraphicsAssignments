#version 330 core

in vec3 fVertex;
in vec3 fNormal;

uniform mat4 model_inv;
uniform samplerCube cube_map;
out vec4 fragColor;

void main(void) { 
       //material color
       //fragColor = vec4(abs(normalize(fVertex)), 1.0);
       vec4 materialColor = 0.4*vec4(abs((fNormal+1)/2), 1.0);
	
       //reflected color
       vec3 incident = normalize(-fVertex);
       vec4 reflected = model_inv*vec4(reflect(incident, fNormal),0.0);
       vec4 reflectedColor = texture(cube_map, vec3(reflected));
	
	
	//TODO
	
        //refraction
        float eta = 1.52;
        float r = 1.0/eta;
        vec3 refracted = refract(incident, fNormal, r);
        refracted = vec3(model_inv* vec4(refracted, 0.0));
        vec4 refractedColor = texture(cube_map, refracted);
	
	// Schlicks approximation
        float r_s = pow((1 - eta)/(1 + eta), 2);
        float shlick = r_s + (1 - r_s)*pow((1 + dot(fNormal, incident)), 5);
	
        //final color
        // With shlick - final
	fragColor = (1-shlick)*refractedColor + shlick*reflectedColor;

        // Only refracted
        // fragColor = refractedColor;
}       
