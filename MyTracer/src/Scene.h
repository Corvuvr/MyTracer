#pragma once
#include "CL/cl.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Ray.h"

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 0.0f;
	float Metallic = 0.0f;
};

struct Sphere
{
	mutable glm::vec3 Pos{ 0.0f };
	float Radius = 0.5f;
	Material Mat;
};

struct Triangle
{
	cl_float3 v0;
	cl_float3 v1;
	cl_float3 v2;
};

class BoundingBox
{
public:
	BoundingBox() {};
	BoundingBox(glm::vec3 maxA, glm::vec3 minB)
	{
		this->verts[0] = maxA;
		this->verts[1] = glm::vec3(maxA.x, maxA.y, minB.z);
		this->verts[2] = glm::vec3(maxA.x, minB.y, minB.z);
		this->verts[3] = glm::vec3(maxA.x, minB.y, maxA.z);
		this->verts[4] = glm::vec3(minB.x, maxA.y, maxA.z);
		this->verts[5] = glm::vec3(minB.x, maxA.y, minB.z);
		this->verts[6] = minB;
		this->verts[7] = glm::vec3(minB.x, minB.y, maxA.z);
	}
	bool has_intersect(const Ray& ray) const
	{
		// проверка по шести граням
		if (!is_inside_facet(0, 1, 2, 3, ray)) return false;
		if (!is_inside_facet(4, 5, 1, 0, ray)) return false;
		if (!is_inside_facet(7, 6, 5, 4, ray)) return false;
		if (!is_inside_facet(3, 2, 6, 7, ray)) return false;
		if (!is_inside_facet(0, 3, 7, 4, ray)) return false;
		if (!is_inside_facet(1, 5, 6, 2, ray)) return false;
	}

private:
	glm::vec3 verts[8];
	bool is_inside_facet(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, const Ray& ray) const
	{
		uint8_t i = 0;
		glm::vec3 N = glm::cross(verts[i + 1] - verts[i], verts[i + 2] - verts[i]);
		float det = glm::dot(N, ray.Direction);
		if (det == 0) return false;
		float t = -(glm::dot(N, ray.Origin) + glm::dot(-N, verts[i])) / det;
		if (t < 0) return false; // плоскость позади камеры

		glm::vec3 P = ray.Origin + t * ray.Direction;

		if (glm::dot(
			glm::cross(verts[v0] - verts[v1], P - verts[v1]), N
		) < 0)
		{
			return false;
		}
		if (glm::dot(
			glm::cross(verts[v1] - verts[v2], P - verts[v2]), N
		) < 0)
		{
			return false;
		}
		if (glm::dot(
			glm::cross(verts[v2] - verts[v3], P - verts[v3]), N
		) < 0)
		{
			return false;
		}
		if (glm::dot(
			glm::cross(verts[v3] - verts[v0], P - verts[v0]), N
		) < 0)
		{
			return false;
		}
		return true;
	}
};

struct Mesh
{
	Mesh(){}
	std::vector<struct Triangle> Triangles;
	Material Mat;
	void loadMesh(const char* filename);
	BoundingBox boundingBox;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Mesh> Meshes;
};