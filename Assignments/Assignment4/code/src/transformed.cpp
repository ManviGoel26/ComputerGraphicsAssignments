//transformed.cpp

#include "transformed.h"
#include "imgui_setup.h"

bool Transformed::intersect(Ray& r) const
{
	// Calculate New Ray
	glm::vec3 newOr = glm::inverse(transform)*glm::vec4(r.getOrigin().e[0], r.getOrigin().e[1], r.getOrigin().e[2], 1.0f);
	glm::vec3 newDr = glm::inverse(transform)*glm::vec4(r.getDirection().e[0], r.getDirection().e[1], r.getDirection().e[2], 1.0f);
	Vector3D origin(newOr.x, newOr.y, newOr.z);
	Vector3D dir(newDr.x, newDr.y, newDr.z);
    Ray nr(origin, dir);
	
	// Intersect suing the new ray
	bool b = object->intersect(nr);

	// Transofrm intersection
	Vector3D post = nr.getPosition();
	glm::vec3 newPost = transform*glm::vec4(post.e[0], post.e[1], post.e[2], 1.0f);
	r.setPosition(Vector3D(newPost.x, newPost.y, newPost.z), this);
	
	// Transform Normal
	glm::vec3 newNormal = glm::transpose(glm::inverse(transform))*glm::vec4(nr.getNormal().e[0], nr.getNormal().e[1], nr.getNormal().e[2], 1.0f);
	r.setNormal(Vector3D(newNormal.x, newNormal.y, newNormal.z));

	// Return
	return b;
	
}


