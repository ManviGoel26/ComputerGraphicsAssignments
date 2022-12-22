//Implicit surface.h
#ifndef _IMPLICITSURFACE_H_
#define _IMPLICITSURFACE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class ImplicitSurface : public Object
{
    private:

    public:
	    ImplicitSurface(Material* mat):
		    Object(mat)
	    {
		    isSolid = true;
	    }
	
	    virtual bool intersect(Ray& r) const;
        float value(Vector3D p) const;
		Vector3D calculateNormal(Vector3D p) const;

};
#endif
