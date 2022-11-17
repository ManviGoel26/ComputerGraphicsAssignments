#include "world.h"
#include "material.h"
#include "imgui_setup.h"
#include<algorithm>
#include "color.h"

#define MAX_DEPTH 3

Color Material::shade(const Ray& incident, const bool isSolid, int depth) const
{
    Color obColor(0);
    Color ambient(color*ka);
    obColor = obColor + ambient;

    for (int i = 0; i < world->getLightSource().size(); i++)
    {
        bool shaded = false;
        Vector3D normal = incident.getNormal();
        
        // Shadow Ray
		Ray shadowRay(incident.getPosition(), world->getLightSource()[i]->getPosition()-incident.getPosition());
		world->firstIntersection(shadowRay);
		if (shadowRay.didHit()) shaded = true;
        
        
        // diffuse
        normal.normalize();
        Vector3D l(world->getLightSource()[i]->getPosition() - incident.getPosition()); l.normalize();
        float diffuse_product = dotProduct(normal, l);
        Color diff1(world->getLightSource()[i]->getIntensity()*kd);
        Color diff2(color*std::max(diffuse_product, 0.0f));
        Color diffuse(diff1*diff2);

        // specular
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

        if (kr > 0 && depth < MAX_DEPTH)
        {
            double mm = 2*dotProduct(inc, normal);
            Ray reflectedRay(incident.getPosition(), inc - normal*mm);
            Color c(world->shade_ray(reflectedRay, depth+1)*kr);
            obColor = obColor + c;
        }

        
        if (kt > 0 && depth < MAX_DEPTH)
        {
            Ray refractedRay(incident.getPosition(), eta*(inc - normal*dotProduct(inc, normal)) - normal*pow(1-(pow(eta, 2)*(1-pow(dotProduct(inc, normal), 2))), 0.5));
            Color rr(world->shade_ray(refractedRay, depth+1)*kt);
            obColor = obColor + rr;
            
        }
	}

    return obColor;
}
