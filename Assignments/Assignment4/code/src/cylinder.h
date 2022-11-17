//Cylinder.h
#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Cylinder : public Object
{
private:
// Center, radius and height 
	Vector3D center;
    double radius;
    double height;

public:
	Cylinder(const Vector3D& _center, double _rad, double _height, Material* mat):
		Object(mat), center(_center), radius(_rad), height(_height)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
};
#endif
