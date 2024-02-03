#include "Perspective.h"

void Perspective::generateInitRayArr() {

	int x;
	int y;
	vec3 pointOnScreen;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {

			// Calculate point on screen
			pointOnScreen = screenCorners.bottomLeft + vec3{ (x, y, 0) };

			// Calculate rayVec from origin to screen and add to rayArr
			initRayArr[x][y].direction = { pointOnScreen - origin };
			initRayArr[x][y].direction.normalise();
			initRayArr[x][y].origin = { origin };

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
