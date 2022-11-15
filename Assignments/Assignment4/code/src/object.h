//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"
#include "world.h"
// #include "lightsource.h"
// #include "pointlightsource.h"

class Object
{
protected:
	Material *material;
	bool isSolid;
	Vector3D normal;

public:
	Object(Material *mat): material(mat) {}	
	virtual bool intersect(Ray& ray) const = 0;
	virtual Color shade(const Ray& ray, Vector3D lightDir, Color lightInt, Vector3D point, bool shaded) const
	{
		return material->shade(ray, isSolid, normal, lightDir, lightInt, point, shaded);
	}
	Vector3D getNormal() const
	{
		return normal;
	}
};

#endif
