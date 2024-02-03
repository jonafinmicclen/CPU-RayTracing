#pragma once

struct vec3 {
	float x;
	float y;
	float z;
};

struct ray {
	vec3 origin;
	vec3 direction;
	vec3 color;
	float length;
};