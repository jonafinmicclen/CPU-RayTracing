#include "Perspective.h"

void Perspective::generateInitRayArr() {

	double x;
	double y;
	vec3 pointOnScreen;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			// Calculate point on screen
			pointOnScreen = screenCorners.bottomLeft + vec3{ x*10.0/WIDTH, y*10.0/HEIGHT, 0 };

			// Calculate rayVec from origin to screen and add to rayArr
			initRayArr[int(x)][int(y)].direction = { pointOnScreen - origin };
			initRayArr[int(x)][int(y)].direction.normalise();
			initRayArr[int(x)][int(y)].origin = { origin };

		}
	}
}

void Perspective::calculateScreenArr(const triangularModel model) {

	vec3 intersectionPoint;
	ray currentRay;
	double distance;
	double minDistance, maxDistance;
	vec3 color;
	int x, y;
	bool hitLight = false;
	bool intersected = false;
	int bounces;
	bool done = false;



	vec3 lastNormal, lastIntersectionPoint;

	// Define a light source for shading
	lightSource lSource;
	lSource.color = { 1,1,1 };
	lSource.position = { 15,10,-30};
	lSource.radius = 4;


	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			currentRay = initRayArr[x][y];
			for (bounces = 0; bounces < 6; ++bounces) {

				// Reset values before ray hits anything
				color = { 0,0,0 };
				minDistance = 1000000000000;

				if (intersectRayLightSource(currentRay, lSource, distance)) {
					if (distance < minDistance) {
						hitLight = true;
						minDistance = distance;
						break;	// Does this break out of the loop
					}
				}

				// Does it hit the model
				for (auto& tri : model.models) {
					if (intersectRayTriangle(currentRay, tri, distance, intersectionPoint)) {
						if (distance < minDistance) {

							// Variables to track
							intersected = true;
							minDistance = distance;

							// Variables of color information
							color = tri.material.color_RGB;
							lastNormal = tri.normal;
							lastIntersectionPoint = intersectionPoint;
							
						}
					}
				}


				if (intersected) {
					currentRay = currentRay.reflect(lastNormal, lastIntersectionPoint);
					currentRay.origin = currentRay.origin + currentRay.direction*0.001;
					currentRay.distanceTraveled += minDistance;
					currentRay.color_RGB = color;
					intersected = false;
				}
				else {
					break;
				}
			}

			if (hitLight) {
				ScreenArr[x][y] = ( currentRay.color_RGB * 3 ) / std::pow(currentRay.distanceTraveled,2);
			}
		}
	}
}

void Perspective::clearScreen()
{
	int x, y;
	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			ScreenArr[x][y] = { 0,0,0 };

		}
	}
}