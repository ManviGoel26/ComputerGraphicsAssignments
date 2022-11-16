//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"
#include "world.h"
#include "string.h"
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
	virtual Color shade(const Ray& ray, int depth) const
	{
		return material->shade(ray, isSolid, normal, depth);
	}
	Vector3D getNormal() const
	{
		return normal;
	}
};

#endif
