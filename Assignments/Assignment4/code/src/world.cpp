#include "world.h"
#include "iostream"

using namespace std;

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
	{
		objectList[i]->intersect(ray);
	}
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray)
{
	firstIntersection(ray);
	if(ray.didHit())
	{
		Color obColor(0);
		for (int i = 0; i < lightSourceList.size(); i++)
		{
			// Shadow ray 
			bool shaded = false;
			Ray shadowRay(ray.getPosition(), lightSourceList[i]->getPosition()-ray.getPosition());
			firstIntersection(shadowRay);
			if (shadowRay.didHit()) shaded = true;
			obColor = obColor + (ray.intersected())->shade(ray, lightSourceList[i]->getPosition(), lightSourceList[i]->getIntensity(), ray.getPosition(), shaded);
		}
		return obColor;
	}
	return background;
}
