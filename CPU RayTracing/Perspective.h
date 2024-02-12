#pragma once

#include <iostream>
#include "Utility.h"
#include <algorithm>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "ThreadPool.h"


enum SC
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

// Screen corner type
struct ScreenCorners {
	vec3 topLeft;
	vec3 topRight;
	vec3 bottomRight;
	vec3 bottomLeft;
};

class Perspective
{
public:
	// measured in pixels
	static const int WIDTH = 400;
	// measured in pixels
	static const int HEIGHT = 400;

	// perspective origin
	vec3 origin = { 0,0,-2 };

	// Contains screen corner coordinates
	ScreenCorners screenCorners{ { -5,5,10}, {5,5,10}, {5, -5, 10}, {-5,-5,10 }};

	// Contains color values for screen
	vec3 ScreenArr[WIDTH][HEIGHT];
	// initial ray cast array, constant when origin and corners are constant
	ray initRayArr[WIDTH][HEIGHT];
	// Active ray paths
	rayPath rayPaths[WIDTH][HEIGHT];
	// Current scene
	Scene scene;

	// Generates the rays of the inital cast through the screen
	void generateInitRayArr();
	// Calculates pixel values from ray intersections with enviroment
	void calculateScreenArr(const triangularModel model);
	// Temp fill screenS
	void testScreenArrFill();
	void calculatePathsForRow(int x);
	// Clears the screen
	void clearScreen();
	// Resets paths to init rays
	void resetPaths();
	// Calculate a ray path in rayPaths
	void calculatePath(const ivec2 pixelCoord);

private:
	std::thread threads[WIDTH];
	const int SPLIT_WIDTH_THREAD = 50;
	ThreadPool threadPool{ WIDTH/SPLIT_WIDTH_THREAD };
};

