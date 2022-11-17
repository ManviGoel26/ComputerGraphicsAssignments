// transformed.h
#ifndef _TRANSFORMED_H_
#define _TRANSFORMED_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "imgui_setup.h"


class Transformed : public Object
{
private:
    Object *object;
    glm::mat4 transform;

public:
	Transformed(Object* o, Material* mat):
		Object(mat), object(o)
	{

        glm::mat4 m = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 2.0f, 0.0f, 1.0f));
        transform = m;
	}
	
	virtual bool intersect(Ray& r) const;
};
#endif
