#include "world.h"
#include "iostream"

using namespace std;
#define MAX_RAY_DEPTH 3 

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
	{
		objectList[i]->intersect(ray);
	}
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray, int depth)
{
	firstIntersection(ray);
	if(ray.didHit())
	{
		Color obColor((ray.intersected())->shade(ray, depth));
		return obColor;
	}
	
	return background;
}
