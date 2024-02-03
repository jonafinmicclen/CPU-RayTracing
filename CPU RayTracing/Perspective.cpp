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
	double minDistance;
	vec3 color;
	int x, y;
	bool intersected = false;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			currentRay = initRayArr[x][y];
			// Reset color and minimum distance values for calculating nearest color
			minDistance = 100000000;
			color = { 0,0,0 };

			for (auto& tri : model.models) {

				if (intersectRayTriangle(currentRay, tri, distance)) {

					// Only add the color of the nearest intersection with the ray
					if (distance < minDistance) {
						intersected = true;
						color = tri.material.color_RGB;
						minDistance = distance;
					}

				}
			}
			// Add the color to the screen array after calculating correct value
			if (intersected) {
				ScreenArr[x][y] = color;
				intersected = false;
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
