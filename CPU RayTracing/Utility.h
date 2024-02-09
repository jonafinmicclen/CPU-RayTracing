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

	void rotateInPlace(rotator rot) {
		*this = *this + rot;
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

	// Adding a rotation
	vec3 operator+(const rotator& other) const {
		// other.rotationPoint + 
		return returnRotatedVec3(*this - other.rotationPoint, other.angle, other.axis);
	}

	// OS override
	friend std::ostream& operator<<(std::ostream& os, const vec3& vec) {
		os << "x:" << vec.x<< " y:" << vec.y << " z:" << vec.z;
		return os;
	}

};

struct ivec2 {
	int x;
	int y;
};

inline vec3 returnRotatedVec3(const vec3 vec, double angleRadians, const vec3 axis) {

	double cosAngle = cos(angleRadians);
	double sinAngle = sin(angleRadians);

	double oneMinusCos = 1.0 - cosAngle;

	double axisX = axis.x;
	double axisY = axis.y;
	double axisZ = axis.z;

	double newX = (cosAngle + oneMinusCos * axisX * axisX) * vec.x +
		(oneMinusCos * axisX * axisY - sinAngle * axisZ) * vec.y +
		(oneMinusCos * axisX * axisZ + sinAngle * axisY) * vec.z;

	double newY = (oneMinusCos * axisY * axisX + sinAngle * axisZ) * vec.x +
		(cosAngle + oneMinusCos * axisY * axisY) * vec.y +
		(oneMinusCos * axisY * axisZ - sinAngle * axisX) * vec.z;

	double newZ = (oneMinusCos * axisZ * axisX - sinAngle * axisY) * vec.x +
		(oneMinusCos * axisZ * axisY + sinAngle * axisX) * vec.y +
		(cosAngle + oneMinusCos * axisZ * axisZ) * vec.z;

	return vec3({ newX, newY, newZ });
}

struct rotator {
	vec3 axis;
	double angle;
	vec3 rotationPoint = { 0,0,0 };
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
};

// Contains information for updating ray color after a bounce.
struct material {
	// RGB color of a material
	vec3 color_RGB;
};

// Currently unused
struct shadeBuffer {
	// contains where the shade should be applied
	vec3 position;
	// contains the shading information, vec3 for more complex shading filters.
	vec3 shade;
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

	void rotateInPlace(const rotator rot) {
		v1 = v1 + rot;
		v2 = v2 + rot;
		v3 = v3 + rot;
		calculateNormal();
	}

	bool rayIntersection(const ray& r, double& distance, vec3& intersectionPoint) {

		// Check if the ray is parallel to the triangle
		double dotProduct = normal.dot(r.direction);
		if (std::abs(dotProduct) < 1e-20) {
			return false;
		}

		// Calculate the distance from the ray origin to the triangle plane
		double t = normal.dot(v1 - r.origin) / dotProduct;

		// Check if the intersection point is behind the ray origin
		if (t < 0) {
			return false;
		}

		// Calculate the intersection point
		intersectionPoint = r.origin + r.direction * t;

		// Check if the intersection point is inside the triangle
		vec3 edge1 = v2 - v1;
		vec3 edge2 = v3 - v1;
		vec3 edge3 = intersectionPoint - v1;

		double detT = edge1.cross(edge2).dot(normal);
		double u = edge3.cross(edge2).dot(normal) / detT;
		double v = edge1.cross(edge3).dot(normal) / detT;

		distance = (intersectionPoint - r.origin).length();

		return (u >= 0 && v >= 0 && u + v <= 1);
	}
};

// Model of triangular surfaces.
struct triangularModel {

	std::vector<triangle> models;

	// Rotates all tris in place
	void rotateInPlace(const rotator rot) 
	{
		for (triangle& tri : models) {
			tri.rotateInPlace(rot);
		}
	}

	// Sets all tris to have the same material
	void setMaterials(const material mat) {
		for (triangle& tri : models) {
			tri.material = mat;
		}
	}

};

// Model made of vertices and surfaces
struct standardModel {

	// Vertex coordinate for each vertex
	std::vector<vec3> vertices;
	// Vertex index of each surface
	std::vector<std::tuple<int, int, int>> surfaces;
	// Material of each surface, with same indexing
	std::vector<material> materials;

	void translate(const vec3 translation) {
		for (vec3& v : vertices) {
			v = v + translation;
		}
	}

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
			triModel.models.push_back(tri);
			++i;
		}

		return triModel;
	}
};

struct scene {

	// Vector of triangular models in scene
	std::vector<triangularModel> sceneFreeTriModels;
	// Vector of light sources in scene
	std::vector<lightSource> sceneFreeLightSources;
	// Vector of triangular models in scene
	std::vector<triangularModel> sceneFixedTriModels;
	// Vector of light sources in scene
	std::vector<lightSource> sceneFixedLightSources;

	// Rotate free objects
	void rotateScene(rotator rot) {
		for (triangularModel& triMod : sceneFreeTriModels)
		{
			triMod.rotateInPlace(rot);
		}
		for (lightSource& lSrc : sceneFixedLightSources)
		{
			lSrc.position.rotateInPlace(rot);
		}
	}

};

struct lightSource {

	// Center of light source
	vec3 position = { 0,0,0 };
	// Color of light source
	vec3 color = { 1,1,1 };
	// Radius of light source
	double radius = 1;
	// How bright (0-1) not neccessary default 1
	double intensity = 1;

	// Tests ray intersection
	bool rayIntersection(const ray& r) {
		return (2.0f * (r.origin - position).dot(r.direction)) *
			(2.0f * (r.origin - position).dot(r.direction)) - 4 *
			(r.direction.dot(r.direction)) *
			((r.origin - position).dot(r.origin - position)
				- radius * radius) >= 0;
	}

};