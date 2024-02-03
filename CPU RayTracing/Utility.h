#pragma once

#include <cmath>

// Spacial vector of x, y, z. Contains common mathematical methods.
struct vec3 {

	// x coordinate of vector
	double x;
	// y coordinate of vector
	double y;
	// z coordinate of vector
	double z;

	// Reflect with normal and return new vector, current vector unchanged
	vec3 relfect(const vec3& normal) const {
		return *this - (normal * dot(normal) * 2);
	}

	// Reflects this vector with a normal, this vector becomes the reflected one
	void reflectInPlace(const vec3& normal) {
		*this = *this - (normal * dot(normal) * 2);
	}

	// Returns the normalised vector
	vec3 normalised() const {
		double length = (*this).length();
		if (length == 0) {
			throw std::runtime_error("Cannot normalise vector of length zero.");
		}
		return *this / length;
	}

	// Normalises the vector in place
	void normalise() {
		double length = (*this).length();
		if (length == 0) {
			throw std::runtime_error("Cannot normalise vector of length zero.");
		}
		*this = *this / length;
	}

	// Returns the dot product
	float dot(const vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// Returns the euclidian length
	double length() const {
		return std::sqrt((std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2)));
	}

	// Pointwise addition
	vec3 operator+(const vec3& other) const {
		return vec3({ x + other.x, y + other.y, z + other.z });
	}

	// Pointwise subtraction
	vec3 operator-(const vec3& other) const {
		return vec3({ x - other.x, y - other.y, z - other.z });
	}

	// Scalar division
	vec3 operator/(const double& other) const {
		return vec3({ x / other, y / other, z / other });
	}

	// Scalar multiplication
	vec3 operator*(const double& other) const {
		return vec3({ x * other, y * other, z * other });
	}

	// OS override
	friend std::ostream& operator<<(std::ostream& os, const vec3& vec) {
		os << "x:" << vec.x<< " y:" << vec.y << " z:" << vec.z;
		return os;
	}
};

// Represents a 'multiple' photon as a ray with distance traveled for intensity approximation.
struct ray {
	// ray origin (starting point)
	vec3 origin;
	// ray direction vector
	vec3 direction;
	// ray color, RGB
	vec3 color;
	// ray distance traveled, to calculate itensity
	float distanceTraveled;
};

// Represents a triangular structure to make up larger visual objects.
struct triangle {
	// vertex 1
	vec3 v1;
	// vertex 2
	vec3 v2;
	// vertex 3
	vec3 v3;
	// color, RGB
	vec3 color;
};
