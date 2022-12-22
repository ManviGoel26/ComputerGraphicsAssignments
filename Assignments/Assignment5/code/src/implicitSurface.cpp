//implicitSurface.cpp

#include "implicitSurface.h"
#include "iostream"

float ImplicitSurface::value(Vector3D p) const
{
    double x = p.e[0];
    double y = p.e[1];
    double z = p.e[2];

    // Equation of the implicit function
    return 2*y*(y*y - 3*x*x)*(1-z*z) + pow(x*x+y*y, 2) - (9*z*z - 1)*(1-z*z);
    //  - (9*pow(z, 2) - 1)*(1-pow(z, 2));
}

Vector3D ImplicitSurface::calculateNormal(Vector3D p) const
{   
    double x = p.e[0];
    double y = p.e[1];
    double z = p.e[2];

    // Calculating partial derivatinves wrt variables
    double dfbydx = 2*y*(1-z*z)*(-6*x) + 2*(x*x + y*y)*(2*x);
    double dfbydy = 2*(y*y - 3*x*x)*(1-z*z) + 2*y*(1-z*z)*2*(y) + 2*(x*x + y*y)*(2*y);
    double dfbydz = -2*z*(2*y*(y*y - 3*x*x)) - 18*z*(1-z*z) + 2*z*(9*z*z - 1);

    // Vector3D normal(2*x, -2*y, 0);
    // Calculating the normal
    Vector3D normal(dfbydx, dfbydy, dfbydz);
    return normal;
}

bool ImplicitSurface::intersect(Ray& r) const
{
    Vector3D o = r.getOrigin();
    Vector3D d = r.getDirection();

    float maxDist = 100;
    float stepSize = 0.001;
    float t = 0;
    
    Vector3D start = o + t*d;
    bool intersect = false;
    bool currentSign = std::signbit(value(start));

    // Ray Marching algorithm
    while (t < maxDist || intersect == false)
    {
        // Previous value
        Vector3D prev = o + t*d;

        // Update t value
        t += stepSize;

        // Next value
        Vector3D next = o + t*d;
        if (std::signbit(value(next)) != currentSign)
        {
            // change of sign
            intersect = true;
            // float t_final = ((prev - o).e[0])/d.e[0];
            // r.setParameter(t_final, this);
            // r.setNormal(calculateNormal(prev));
            // return true;
            
            while (1)
            {
                Vector3D m = (prev + next)/2;
                
                float signedDistance = value(m);
                if (m.e[0] == prev.e[0] && m.e[1] == prev.e[1] && m.e[2] == prev.e[2])
                {
                    break;
                }
                
                if (fabs(signedDistance) < 0.1)
                {
                    float t_final = ((m - o).e[0])/d.e[0];
                    r.setParameter(t_final, this);
                    r.setNormal(calculateNormal(m));
                    return true;
                }

                if (signedDistance < 0)
                {
                    if (value(prev) < 0)
                    {
                        prev = m;
                    }
                    else
                    {
                        next = m;
                    }
                }
                else
                {
                    if (value(prev) < 0)
                    {
                        next = m;
                    }
                    else
                    {
                        prev = m;
                    }
                }
            }    

        }
    }

    
    
	return false;
}

