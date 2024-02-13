#include "Perspective.h"

void Perspective::generateInitRayArr() {

	// Point on virtual screen which rays path through
	vec3 pointOnScreen;

	double x;
	double y;
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
	resetPaths();
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
				for (auto& tri : model.tris) {
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

void Perspective::testScreenArrFill() {
	for (int x = 0; x < WIDTH / SPLIT_WIDTH_THREAD; ++x) {
		threadPool.enqueue([this, x] {
			calculatePathsForRow(x * SPLIT_WIDTH_THREAD);
			});
	}
}

void Perspective::calculatePathsForRow(int x) {
	for (int y = 0; y < HEIGHT; ++y) {
		for (int b = 0; b < SPLIT_WIDTH_THREAD; ++b) {
			calculatePath({ x + b, y });
		}
	}
}
void Perspective::drawST()
{
	int x, y;
	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) 
		{

			calculatePath({ x, y });

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

void Perspective::resetPaths()
{
	int x, y;
	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) 
		{	
			// Reset first ray default values
			rayPaths[x][y].segment[0] = initRayArr[x][y];
		}
	}
}

void Perspective::calculatePath(const ivec2 pixelCoord)
{
	rayPath* currentPath = &rayPaths[pixelCoord.x][pixelCoord.y];

	int max_segments = 6;
	int current_segment_index;

	vec3 intersectionPoint;
	vec3 closest_intersection;
	vec3 closest_normal;
	double distance;

	// Closest triangle on this pass
	double closest_distance = 999999999999999;
	bool intersected;
	bool done = false;

	currentPath->illuminated = false;
	ray* current_segment = &currentPath->segment[0];

	for (current_segment_index = 0; current_segment_index < max_segments; ++current_segment_index, ++current_segment) 
	{
		// Reset flags
		done = false;
		closest_distance = 999999999999999;
		intersected = false;
		

		// Loop on triangles
		for (const triangularModel& model : scene.models) {
			for (const triangle& tri : model.tris) 
			{
				if (current_segment->intersectTriangle(tri, distance, intersectionPoint))
				{
					if (distance < closest_distance) 
					{
						intersected = true;
						closest_intersection = intersectionPoint;
						closest_normal = tri.normal;
						closest_distance = distance;
						currentPath->intersected = true;
						current_segment->color_RGB = tri.material.color_RGB;
						current_segment->distanceTraveled = distance;
					}
				}
			}
		}

		// Loop on light sources 
		for (const lightSource& lSource : scene.light_sources) 
		{
			if (current_segment->intersectLightSource(lSource, distance))
			{
				if (distance < closest_distance) 
				{
					
					currentPath->illuminated = true;
					currentPath->completion_index = current_segment_index;
					current_segment->color_RGB = { 1,1,1 };
					current_segment->distanceTraveled = distance;
					done = true;
				}
			}
		}

		if (done) {
			break;
		}

		if (current_segment_index < max_segments - 1 && intersected) 
		{
			// Set the next ray to continue from last
			*(current_segment + 1) = current_segment->reflect(closest_normal, closest_intersection);
			(current_segment + 1)->origin = (current_segment + 1)->origin + (current_segment + 1)->direction * 0.000000000001;
		}
	}

	// Temp fill screen
	if (currentPath->illuminated == true) {
		ScreenArr[pixelCoord.x][pixelCoord.y] = (currentPath->segment[0].color_RGB / std::pow( currentPath->getTotalLength(), 2 )) * BRIGHTNESS; /// ; std::pow(currentPath->getTotalLength(), 2)
	}
	else {
		ScreenArr[pixelCoord.x][pixelCoord.y] = { 0,0,0 };
	}
}
