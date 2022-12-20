// triangle.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Triangle : public Object
{
private:
// The three vertices.
	Vector3D a;
	Vector3D b;
    Vector3D c;

public:
	Triangle(const Vector3D& _a, const Vector3D& _b, const Vector3D& _c, Material* mat):
		Object(mat), a(_a), b(_b), c(_c)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
};
#endif
