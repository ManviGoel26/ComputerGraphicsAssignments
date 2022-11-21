#include "ray.h"
#include "iostream"

bool Ray::setParameter(const float par, const Object *obj)
{
	if(par < t && par > SMALLEST_DIST)
	{
		hit = true;
		t = par;
		object = obj;
		return true;
	}
	return false;
}

void Ray::setNormal(const Vector3D _normal)
{
	normal = _normal;
}

