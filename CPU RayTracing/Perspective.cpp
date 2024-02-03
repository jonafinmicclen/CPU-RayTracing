#include "Perspective.h"

void Perspective::generateInitRayArr() {

	double x;
	double y;
	vec3 pointOnScreen;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			// Calculate point on screen
			pointOnScreen = screenCorners.bottomLeft + vec3{ x/10.0, y/10.0, 0 };

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

	int x, y;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			currentRay = initRayArr[x][y];

			for (auto& tri : model.model) {

				if (intersectRayTriangle(currentRay, tri)) {

					ScreenArr[x][y] = { 1,1,1 };

				}
			}
		}
	}

}
