#pragma once

#include <iostream>
#include "Utility.h"
#include <algorithm>


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
	static const int WIDTH = 100;
	// measured in pixels
	static const int HEIGHT = 100;

	// perspective origin
	vec3 origin = { 0,0,-10 };

	// Contains screen corner coordinates
	ScreenCorners screenCorners{ { -5,5,10}, {5,5,10}, {5, -5, 10}, {-5,-5,10 }};

	// Contains color values for screen
	vec3 ScreenArr[WIDTH][HEIGHT];
	// initial ray cast array, constant when origin and corners are constant
	ray initRayArr[WIDTH][HEIGHT];

	// Generates the rays of the inital cast through the screen
	void generateInitRayArr();
	// Calculates pixel values from ray intersections with enviroment
	void calculateScreenArr(const triangularModel model);
	// Clears the screen
	void clearScreen();
};

