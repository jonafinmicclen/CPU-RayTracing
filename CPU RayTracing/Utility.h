#pragma once

#include <cmath>
#include <vector>
#include <tuple>

// Spacial vector of x, y, z. Contains common mathematical methods.
struct vec3 {

	// x coordinate of vector
	double x;
	// y coordinate of vector
	double y;
	// z coordinate of vector
	double z;


	// Reflect with normal and return new vector, current vector unchanged
	vec3 reflect(const vec3& normal) const {
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
	double dot(const vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// Returns the cross product
	vec3 cross(const vec3& other) const {
		return vec3({
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
			});
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

struct lightSource {

	// Center of light source
	vec3 position;
	// Color of light source
	vec3 color;
	// Radius of light source
	double radius;
	// How bright (0-1) not neccessary default 1
	double intensity = 1;

};

// Contains information for updating ray color after a bounce.
struct material {
	// RGB color of a material
	vec3 color_RGB;
};

// Represents a trianglular surface/face.
struct triangle {
	// vertex 1
	vec3 v1;
	// vertex 2
	vec3 v2;
	// vertex 3
	vec3 v3;
	// material info
	material material;
	// normal of the surface
	vec3 normal;
	// contains information about the shading on the surface


	// Calculates the normal from the 3 vertices
	void calculateNormal() {
		try {
			normal = (v2 - v1).cross(v3 - v1).normalised();
		}
		catch (std::exception& e) {
			std::cerr << "Error calculating normal: " << e.what() << std::endl;
		}
	}
};

// Represents a 'multiple' photon as a ray with distance traveled for intensity approximation.
struct ray {
	// ray origin (starting point)
	vec3 origin;
	// ray direction vector
	vec3 direction;
	// ray color, RGB
	vec3 color_RGB;
	// ray distance traveled, to calculate itensity
	double distanceTraveled;

	// returns the reflected ray
	ray reflect(const vec3& normal, const vec3& intersectionPoint) {

		ray newRay = *this;
		newRay.direction = direction - normal.normalised() * 2.0 * direction.dot(normal.normalised());
		newRay.origin = intersectionPoint;
		return newRay;
	}

	bool intersectTriangle(const triangle& tri, double& distance, vec3& intersectionPoint) {

		// Check if the ray is parallel to the triangle
		double dotProduct = tri.normal.dot(direction);
		if (std::abs(dotProduct) < 1e-20) {
			return false;
		}

		// Calculate the distance from the ray origin to the triangle plane
		double t = tri.normal.dot(tri.v1 - origin) / dotProduct;

		// Check if the intersection point is behind the ray origin
		if (t < 0) {
			return false;
		}

		// Calculate the intersection point
		intersectionPoint = origin + direction * t;

		// Check if the intersection point is inside the triangl
		vec3 edge1 = tri.v2 - tri.v1;
		vec3 edge2 = tri.v3 - tri.v1;
		vec3 edge3 = intersectionPoint - tri.v1;

		double detT = edge1.cross(edge2).dot(tri.normal);
		double u = edge3.cross(edge2).dot(tri.normal) / detT;
		double v = edge1.cross(edge3).dot(tri.normal) / detT;

		distance = (intersectionPoint - origin).length();

		return (u >= 0 && v >= 0 && u + v <= 1);
	}

	bool intersectLightSource(const lightSource& src, double& distance) {

		// Check if the ray intersects the light source using a quadratic equation
		bool doesIntersect = (2.0 * (origin - src.position).dot(direction)) *
			(2.0 * (origin - src.position).dot(direction)) - 4 *
			(direction.dot(direction)) *
			((origin - src.position).dot(origin - src.position)
				- src.radius * src.radius) >= 0;

		// If there is an intersection, calculate the distance from the ray's origin to the intersection point
		if (doesIntersect) {
			// Calculate the distance from the ray's origin to the intersection point
			distance = (origin - src.position).length() - sqrt((origin - src.position).dot(origin - src.position) - src.radius * src.radius);
		}

		return doesIntersect;
	}

};

struct rayPath
{	// Path of max 6 bounces
	ray segment[6];
	bool illuminated = false;
	bool intersected = false;
	int completion_index;

	double getTotalLength() {
		double total = 0;
		int index = 0;
		for (index = 0; index <= completion_index; ++index) {
			total += segment[index].distanceTraveled;
		}
		return total;
	}
};

struct ivec2
{
	int x;
	int y;
};

// Currently unused
struct shadeBuffer {
	// contains where the shade should be applied
	vec3 position;
	// contains the shading information, vec3 for more complex shading filters.
	vec3 shade;
};



// Model of triangular surfaces.
struct triangularModel {
	std::vector<triangle> tris;
	std::vector<material> materials;
};

// Model made of vertices and surfaces
struct standardModel {

	// Vertex coordinate for each vertex
	std::vector<vec3> vertices;
	// Vertex index of each surface
	std::vector<std::tuple<int, int, int>> surfaces;
	// Material of each surface, with same indexing
	std::vector<material> materials;

	// return triangularModel type version of this
	triangularModel convertToTriModel() const {

		// Initalises the model
		triangularModel triModel;
		int i = 0;

		for (auto& surface : surfaces) {

			triangle tri;

			// Create tri object for poly in the model
			tri.v1 = vertices[std::get<0>(surface)];
			tri.v2 = vertices[std::get<1>(surface)];
			tri.v3 = vertices[std::get<2>(surface)];
			tri.calculateNormal();
			tri.material.color_RGB = materials[i].color_RGB;

			// Add the tri object to the new model
			triModel.tris.push_back(tri);
			++i;
		}

		return triModel;
	}
};

inline bool intersectRayLightSource(const ray& r, const lightSource& src, double& distance) {

		return (2.0f * (r.origin - src.position).dot(r.direction)) * 
			(2.0f * (r.origin - src.position).dot(r.direction)) - 4 * 
			(r.direction.dot(r.direction)) * 
			((r.origin - src.position).dot(r.origin - src.position) 
				- src.radius * src.radius) >= 0;
}

inline bool intersectRayTriangle(const ray& r, const triangle& tri, double& distance, vec3& intersectionPoint) {
	// Calculate the normal of the triangle
	vec3 normal = tri.normal;

	// Check if the ray is parallel to the triangle
	double dotProduct = normal.dot(r.direction);
	if (std::abs(dotProduct) < 1e-20) {
		return false;
	}

	// Calculate the distance from the ray origin to the triangle plane
	double t = normal.dot(tri.v1 - r.origin) / dotProduct;

	// Check if the intersection point is behind the ray origin
	if (t < 0) {
		return false;
	}

	// Calculate the intersection point
	intersectionPoint = r.origin + r.direction * t;

	// Check if the intersection point is inside the triangle
	vec3 edge1 = tri.v2 - tri.v1;
	vec3 edge2 = tri.v3 - tri.v1;
	vec3 edge3 = intersectionPoint - tri.v1;

	double detT = edge1.cross(edge2).dot(normal);
	double u = edge3.cross(edge2).dot(normal) / detT;
	double v = edge1.cross(edge3).dot(normal) / detT;

	distance = (intersectionPoint - r.origin).length();

	return (u >= 0 && v >= 0 && u + v <= 1);
}

struct Scene
{
	std::vector<triangularModel> models;
	std::vector<lightSource> light_sources;
};

// Mathematical sigmoid function
inline double sigmoid(const double x) {
	return 1 / (1 + std::pow(2.71828, -x));
}