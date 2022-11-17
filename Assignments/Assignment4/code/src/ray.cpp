#include "ray.h"
#include "iostream"

bool Ray::setParameter(const float par, const Object *obj)
{
	if(par < t && par > SMALLEST_DIST)
	{
		hit = true;
		t = par;
		object = obj;
		position = origin + t*direction;
		return true;
	}
	return false;
}

void Ray::setNormal(const Vector3D _normal)
{
	normal = _normal;
}

void Ray::setPosition(const Vector3D transformedPosition, const Object *obj)
{
	float par = (transformedPosition.e[0]-origin.e[0])/direction.e[0];
	if(par < t && par > SMALLEST_DIST)
	{
		hit = true;
		position = transformedPosition;
		object = obj;
	}
}
