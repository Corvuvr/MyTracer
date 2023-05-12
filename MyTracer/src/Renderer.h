#pragma once
#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"  

class Renderer
{
public:
	Renderer() = default;
	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	struct HitPayLoad
	{
		float HitDistance;
		float u = 0, v = 0;
		int meshIndex, triangleIndex;
		glm::vec3 WorldPos;
		glm::vec3 WorldNormal;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y); //  ray gen
	
	HitPayLoad TraceRay(const Ray& ray);
	HitPayLoad ClosestHit(const Ray& ray, float hitDistance, int meshIndex, int triangleIndex, float u, float v, glm::vec3 N);
	HitPayLoad Miss(const Ray& ray);

private:
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	std::vector<uint32_t> m_ImageHorisontalIter, m_ImageVerticalIter;

};
