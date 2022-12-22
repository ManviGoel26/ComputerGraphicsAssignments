//matrial.h
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "color.h"
#include "ray.h"
#include <vector>


class World;

class Material
{
private:
	World *world;
public:
	//Data

	Color color;
	double ka;//Ambient contribution
	double kd;//Diffuse constant
	double ks;//Specular constant
	double kr;//Contribution from reflection
	double kt;//Contribution from refraction
	double eta;//Coefficient of refraction
	double n;//Phong's shiny constatnt
	std::vector<unsigned char> image; // texture image
	double im_width;
	double im_height;

	Material(World *w):
		world(w), color(0),
		ka(0), kd(0.0), ks(0), kr(0), kt(0),n(0), eta(0) {}
	// Color shade(const Ray& incident, const bool isSolid) const;
	Color shade(const Ray& incident, const bool isSolid, int depth) ;
	Color getBaseColor(Vector3D p) ;
};
#endif
