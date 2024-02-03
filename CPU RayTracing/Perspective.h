#pragma once

#include <iostream>
#include "Utility.h"

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
	static const int WIDTH = 60;
	// measured in pixels
	static const int HEIGHT = 60;

	// perspective origin
	vec3 origin = { 0,0,0 };

	// Contains screen corner coordinates
	ScreenCorners screenCorners{ { -5,5,10}, {5,5,10}, {5, -5, 10}, {-5,-5,10 }};

	// Contains color values for screen
	vec3 ScreenArr[WIDTH][HEIGHT];
	// initial ray cast array, constant when origin and corners are constant
	ray initRayArr[WIDTH][HEIGHT];

	void generateInitRayArr();
	void calculateScreenArr(const triangularModel model);
};

