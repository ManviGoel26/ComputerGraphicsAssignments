#include "world.h"
#include "material.h"
#include "imgui_setup.h"
#include<algorithm>
#include "color.h"

#define MAX_DEPTH 3

Color Material::getBaseColor(Vector3D p) 
{
    const size_t RGB = 3;
    
    double x = p.e[0];
    double y = p.e[1];
    double z = p.e[2];

    //Calculate u and v
	float theta = acos(z);
    float phi = atan2(y, x + 1);
    int u = im_height*(phi + glm::pi<float>())/2*glm::pi<float>();
    int v = im_width*theta/glm::pi<float>();
   
    //Find the color in the image using u and v
    size_t index = RGB*(v*im_width + u);
    float R = static_cast<int>(image[index + 0]);
    float G = static_cast<int>(image[index + 1]);
    float B = static_cast<int>(image[index + 2]);
    return Color(R/255, G/255, B/255);
}

Color Material::shade(const Ray& incident, const bool isSolid, int depth) 
{
    Color obColor(0);
    Color ambient(color*ka);

    // Ambient Color
    obColor = obColor + ambient;

    // Iterate over all light aourses
    for (int i = 0; i < world->getLightSource().size(); i++)
    {

        bool shaded = false;
        Vector3D normal = incident.getNormal();
        
        // Shadow Ray - create and check
		Ray shadowRay(incident.getPosition(), world->getLightSource()[i]->getPosition()-incident.getPosition());
		world->firstIntersection(shadowRay);
		if (shadowRay.didHit()) shaded = true; // flag
        
        
        // diffuse color
        normal.normalize();
        Vector3D l(world->getLightSource()[i]->getPosition() - incident.getPosition()); l.normalize();
        float diffuse_product = dotProduct(normal, l);
        Color diff1(world->getLightSource()[i]->getIntensity()*kd);
        
        // Set the base color using the texMap
        // color = getBaseColor(incident.getPosition());
        Color diff2(color*std::max(diffuse_product, 0.0f));
        Color diffuse(diff1*diff2);
        
        // specular color
        Vector3D inc(-incident.getDirection()); inc.normalize();
        Vector3D half = inc + l; half.normalize();
        float spec_prod = dotProduct(normal, half);
        spec_prod = std::pow(std::max(spec_prod, 0.0f), n);
        Color specular(world->getLightSource()[i]->getIntensity()*color);
        specular = specular*ks*spec_prod;
        

        // Conditions for shaded
        if (!shaded) 
        {
            obColor = obColor + specular + diffuse;
        }

        // Recursive shading
        if (kr > 0 && depth < MAX_DEPTH)
        {
            // reflected ray
            double mm = 2*dotProduct(inc, normal);
            Ray reflectedRay(incident.getPosition(), inc - normal*mm);
            Color c(world->shade_ray(reflectedRay, depth+1)*kr);
            std::cout << c.B() << " " << c.R() << " " << c.G() << "\n";
            obColor = obColor + c;
        }

        double div = 1/eta;
        if (kt > 0 && depth < MAX_DEPTH)
        {
            // refrated ray
            Ray refractedRay(incident.getPosition(), div*(inc - normal*dotProduct(inc, normal)) - normal*pow(1-(pow(div, 2)*(1-pow(dotProduct(inc, normal), 2))), 0.5));
            Color rr(world->shade_ray(refractedRay, depth+1)*kt);
            obColor = obColor + rr;
            
        }
	}

    return obColor;
}
