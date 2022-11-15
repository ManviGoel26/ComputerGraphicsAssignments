#include "world.h"
#include "material.h"
#include "imgui_setup.h"
#include<algorithm>
#include "color.h"


Color Material::shade(const Ray& incident, const bool isSolid, Vector3D normal, Vector3D lightDir, Color lightInt, Vector3D point, bool shaded) const
{
	Color ambient(color*ka);

    if (shaded) return ambient;
    
    // diffuse
    normal.normalize();
    Vector3D l(lightDir - point); l.normalize();
    float diffuse_product = dotProduct(normal, l);
    Color diff1(lightInt*kd);
    Color diff2(color*std::max(diffuse_product, 0.0f));
    Color diffuse(diff1*diff2);

    // reflect
    Vector3D i(-incident.getDirection()); i.normalize();
    Vector3D half = i + l; half.normalize();
    float spec_prod = dotProduct(normal, half);
    spec_prod = std::pow(std::max(spec_prod, 0.0f), n);
    Color specular(lightInt*color);
    specular = specular*ks*spec_prod;
    
	return ambient + diffuse + specular;
}
