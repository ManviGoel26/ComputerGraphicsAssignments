#include "world.h"
#include "material.h"
#include "imgui_setup.h"
#include<algorithm>
#include "color.h"

#define MAX_DEPTH 3

Color Material::shade(const Ray& incident, const bool isSolid, Vector3D normal, int depth) const
{
    Color obColor(0);
    Color ambient(color*ka);
    obColor = obColor + ambient;

    for (int i = 0; i < world->getLightSource().size(); i++)
    {
        bool shaded = false;
        
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
            obColor = obColor + diffuse + specular;
        }

        if (kr > 0 && depth < MAX_DEPTH)
        {
            double mm = 2*dotProduct(incident.getDirection(), normal);
            Ray reflectedRay(incident.getPosition()+0.01*l, incident.getDirection() - normal*mm);
            obColor = obColor + world->shade_ray(reflectedRay, depth+1)*kr;
        }

        
        if (kt > 0 && depth < MAX_DEPTH)
        {
            Ray refractedRay(incident.getPosition(), eta*(incident.getDirection() - normal*dotProduct(incident.getDirection(), normal)) - normal*pow(1-(pow(eta, 2)*(1-pow(dotProduct(incident.getDirection(), normal), 2))), 0.5));
            obColor = obColor + world->shade_ray(refractedRay, depth+1)*kt;
        }
	}

    return obColor;
}
