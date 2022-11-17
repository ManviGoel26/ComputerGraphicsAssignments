//triangle.cpp

#include "imgui_setup.h"
#include "triangle.h"

bool Triangle::intersect(Ray& r) const
{
	Vector3D e = r.getOrigin();
    Vector3D d = r.getDirection();
    
    glm::mat3 mMatrix(glm::vec3(a.e[0] - b.e[0], a.e[0] - c.e[0], d.e[0]), glm::vec3(a.e[1] - b.e[1], a.e[1] - c.e[1], d.e[1]), glm::vec3(a.e[2] - b.e[2], a.e[2] - c.e[2], d.e[2]));
    glm::vec3 myVector(a.e[0] - e.e[0], a.e[1] - e.e[1], a.e[2] - e.e[2]);
    glm::vec3 beta_gamma = inverse(glm::transpose(mMatrix))*myVector;
    
    if (beta_gamma[0] > 0 && beta_gamma[1] > 0 && beta_gamma[0] + beta_gamma[1] < 1)
    {
        r.setParameter(beta_gamma[2], this);
        r.setNormal(-1*crossProduct(b - a, c - a));
        return true;
    }
	return false;

}
