//cylinder.cpp

#include "cylinder.h"
#include "iostream"

bool Cylinder::intersect(Ray& r) const
{
	// Vector3D centerVector = r.getOrigin() - position;
	double a = pow((r.getDirection().e[0]), 2) + pow((r.getDirection().e[2]), 2);
	double b = 2*((r.getDirection().e[0]*(r.getOrigin().e[0] - center.e[0])) + (r.getDirection().e[0]*(r.getOrigin().e[0] - center.e[0])));
	double c = pow((r.getOrigin().e[0] - center.e[0]), 2) + pow((r.getOrigin().e[2] - center.e[2]), 2) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant >= 0)
		{
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);
            double t = std::min(t1, t2);
            double dist = r.getOrigin().e[2] + t*r.getDirection().e[2];
			
            if ((dist >= center.e[2]) && (dist <= center.e[2] + height))
            {
                bool b = r.setParameter(t, this);
                r.setNormal(unitVector(Vector3D(r.getPosition().e[0] - center.e[0], 0, r.getPosition().e[2] - center.e[2])));
                return b;
            }

            return false;
		}
	}
	return false;

}

