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

void Perspective::calculateScreenArr()
{
}

void Perspective::calculatePixel(const ivec2)
{
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