//implicitSurface.cpp

#include "implicitSurface.h"
#include "iostream"

float ImplicitSurface::value(Vector3D p) const
{
    double x = p.e[0];
    double y = p.e[1];
    double z = p.e[2];

    return x*x + y*y + z*z - 10000;
    // return 2*y*(pow(y, 2) - 3*pow(x, 2))*(1-pow(z, 2)) + pow(pow(x, 2) + pow(y, 2), 2) - (9*pow(z, 2) - 1)*(1-pow(z, 2));
}

Vector3D ImplicitSurface::calculateNormal(Vector3D p) const
{   
    double x = p.e[0];
    double y = p.e[1];
    double z = p.e[2];

    double dfbydx = 2*y*(1-pow(z, 2))*(-6*x) + 2*(pow(x, 2) + pow(y, 2))*(2*x);
    double dfbydy = 2*(pow(y, 2) - 3*pow(x, 2))*(1-pow(z, 2)) + 2*y*(1-pow(z, 2))*2*(y) + 2*(x*x + y*y)*(2*y);
    double dfbydz = -2*z*(2*y*(y*y - 3*x*x)) - 18*z*(1-z*z) + 2*z*(9*z*z - 1);

    Vector3D normal(2*x, 2*y, 2*z);
    
    // Vector3D normal(dfbydx, dfbydy, dfbydz);
    return normal;
}

bool ImplicitSurface::intersect(Ray& r) const
{
    Vector3D o = r.getOrigin();
    Vector3D d = r.getDirection();

    float maxDist = 1;
    float stepSize = 0.5;
    float t = 0;
    
    Vector3D start = o + t*d;
    bool intersect = false;
    bool currentSign = std::signbit(value(start));

    while (t < maxDist || intersect == false)
    {
        std::cout << t << "\n";
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
            std::cout << "Here" << "\n";
            float t_final = ((next - o).e[0])/d.e[0];
            r.setParameter(t_final, this);
            r.setNormal(calculateNormal(next));
            std::cout << t_final << "\n";
            return true;
        
            // while (1)
            // {
            //     Vector3D m = (prev + next)/2;
            //     std::cout << prev.e[0] << " " << m.e[0] << " " << next.e[0] << "\n";
            //     break;
                
            //     float signedDistance = value(m);
            //     // std::cout << signedDistance << " dist" << "\n";
            //     if (fabs(signedDistance) < 0.1)
            //     {
            //         float t_final = ((m - o).e[0])/d.e[0];
            //         r.setParameter(t_final, this);
            //         r.setNormal(calculateNormal(m));
            //         std::cout << t_final << "\n";
            //         return true;
            //     }

            //     if (signedDistance < 0)
            //     {
            //         prev = m;
            //     }
            //     else
            //     {
            //         next = m;
            //     }
            // }    

        }
    }

    
    
	return false;
}

