//sphere.cpp

#include "sphere.h"
#include "imgui_setup.h"

// std::vector<float> getUVMapping(Vector3D p)
// {	
//     double x = p.e[0];
//     double y = p.e[1];
//     double z = p.e[2];

// 	float u = 0.5 + atan2(z, x)/2*glm::pi<float>();
// 	float v = 0.5 - asin(y)/glm::pi<float>();

// 	std::vector<float> ret = {u, v};
// 	return ret;
// }

bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			r.setParameter(t, this);
			r.setNormal(unitVector(r.getPosition() - position));
			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);

			bool b1 = r.setParameter(t1, this);
			bool b2 = r.setParameter(t2, this);
			r.setNormal(unitVector(r.getPosition() - position));

			return b1||b2;    
		}
	}
	return false;

}


