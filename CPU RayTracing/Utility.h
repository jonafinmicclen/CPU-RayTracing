#pragma once

#include <cmath>

struct vec3 {
	double x;
	double y;
	double z;

	// Reflect with normal and return new vector
	vec3 relfect(const vec3& normal) const {
		return *this - (normal * dot(normal) * 2);
	}

	// Reflect with normal in place and modify current vector
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

	// Dot product method
	float dot(const vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// Return euclidian length
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

struct ray {
	vec3 origin;
	vec3 direction;
	vec3 color;
	float length;
};

