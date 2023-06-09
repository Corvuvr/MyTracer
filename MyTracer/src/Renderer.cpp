#include <execution>
#include "Renderer.h"

#include "Walnut/Random.h"

#define TRIANGLES
#define DEBUG_ON

constexpr float kEpsilon = 1e-8;

namespace utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[width * height];

	m_ImageHorisontalIter.resize(width);
	m_ImageVerticalIter.resize(height);

	for (uint32_t i = 0; i < width; ++i)
		m_ImageHorisontalIter[i] = i;

	for (uint32_t i = 0; i < height; ++i)
		m_ImageVerticalIter[i] = i;
}

// функция Render запускается перед каждым новым кадром
void Renderer::Render(const Scene& scene, const Camera& camera) 
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;
	
	const glm::vec3& ray_origin = camera.GetPosition();

	if (m_FrameIndex == 1)
		memset(
			m_AccumulationData, 
			0, 
			m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4)
		);

	std::for_each
	(
		std::execution::par,
		m_ImageVerticalIter.begin(), 
		m_ImageVerticalIter.end(),
		[this](uint32_t y) 
		{
			for (uint32_t x = 0; x < m_FinalImage->GetWidth(); ++x)
			{
				glm::vec4 color = PerPixel(x, y); //<------------------------ ВХОД В PerPixel()
				m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

				glm::vec4 accColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
				accColor /= (float)m_FrameIndex;

				accColor = glm::clamp(accColor, glm::vec4(0.0f), glm::vec4(1.0f));
				m_ImageData[x + y * m_FinalImage->GetWidth()] = utils::ConvertToRGBA(accColor);
			}
		}
	);
	m_FinalImage->SetData(m_ImageData);

	if (m_Settings.Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;
}

glm::vec4 Renderer::PerPixel (uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];
	
	glm::vec3 color(0.0f);
	glm::vec3 colorbuffer(0.0f);
	glm::vec3 color_res(0.0f);
	int bounces = 2;
	float multiplier = 1.0f;
	float shadowMultiplier = 1.0f;
	float fogmultiplier = 0.1f;
	// количество отражений
	for (int i = 0; i < bounces; ++i)
	{
		Renderer::HitPayLoad payload = TraceRay(ray); // <--------------- ВХОД В TraceRay()
		if (payload.HitDistance < 0.0f)
		{
			glm::vec3 SkyColor = glm::vec3(0.0f,0.0f,0.0f);
			color += SkyColor * multiplier;
			break;
		}
		PointLight pl1(glm::vec3(2.0f, 3.0f, 4.0f), 1);
		glm::vec3 lightDir = payload.WorldPos - pl1.Origin;
		float cosangle	=	glm::dot(-lightDir, payload.WorldNormal) 
						/	(glm::length(payload.WorldNormal) * glm::length(lightDir)); 
		cosangle = glm::max(cosangle, 0.0f);
		
		const Mesh& mesh = m_ActiveScene->Meshes[payload.meshIndex];
		// если трегуольник повёрнут лицевой стороной:
		if (glm::dot(ray.Direction, payload.WorldNormal) <= 0)
		{
			color += glm::vec3(cosangle) * pl1.intensity * mesh.Mat.Albedo;

			Ray shadowRay;
			shadowRay.Origin = payload.WorldPos + payload.WorldNormal * 0.0001f;
			shadowRay.Direction = pl1.Origin - shadowRay.Origin 
				+ mesh.Mat.Roughness * Walnut::Random::Vec3(-0.05f, 0.05);

			Renderer::HitPayLoad shadowPayload = TraceRay(shadowRay);//<---ВХОД В TraceRay()
			if (shadowPayload.HitDistance < 1 && shadowPayload.HitDistance > 0)
			{
				color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
			if (i > 0)
			{
				color *= 0.7f * i;
			}
			color_res = (color_res + color) / (float)(i + 1);
		}
		// определяем направление следующего луча (отражение)
		ray.Origin = payload.WorldPos + payload.WorldNormal * 0.0001f; // P = O + t*D
		ray.Direction = glm::reflect
		(
			ray.Direction,
			glm::normalize(
				payload.WorldNormal + mesh.Mat.Roughness 
						* Walnut::Random::Vec3(-0.5f, 0.5)
			)
		);
	}
	return glm::vec4(color_res, 1.0f);
}

Renderer::HitPayLoad Renderer::TraceRay(const Ray& ray)
{
	int closestTriangleIndex = -1, closestMeshIndex = -1;
	float hit_distance = FLT_MAX;
	float u, v, normalU, normalV;
	glm::vec3 planeNormal = glm::vec3(.0f);
	glm::vec3 normalPlaneNormal = glm::vec3(.0f);

	for (size_t i = 0; i < m_ActiveScene->Meshes.size(); i++)
	{
		// bounding box intersiction
		if ((*m_ActiveScene).Meshes[i].boundingBox.has_intersect(ray)) continue;

		for (size_t j = 0; j < m_ActiveScene->Meshes[i].Triangles.size(); j++)
		{
			const Triangle& triangle = m_ActiveScene->Meshes[i].Triangles[j];

			// Алгоритм Мёллера-Трамбора (МТ) - быстрый поиск пересечения с треугольком.

			// реализация уравнения "Луч = плоскость", поиск неизвестных
			glm::vec3 v0v1 = triangle.v1 - triangle.v0;
			glm::vec3 v0v2 = triangle.v2 - triangle.v0;
			glm::vec3 pvec = glm::cross(ray.Direction, v0v2);
			float det = glm::dot(v0v1, pvec);

			if (fabs(det) == 0) continue;

			float invertedDet = 1.0f / det;

			glm::vec3 tvec = ray.Origin - triangle.v0;
			u = glm::dot(tvec, pvec) * invertedDet;
			if ((u < .0f) || (u > 1.0f))
			{
				continue;
			}

			glm::vec3 qvec = glm::cross(tvec, v0v1);
			v = glm::dot(ray.Direction, qvec) * invertedDet;
			if ((v < .0f) || ((u + v) > 1.0f))
			{
				continue;
			}

			float t = glm::dot(v0v2, qvec) * invertedDet;

			if (t < hit_distance && t > 0)
			{
				hit_distance = t;
				closestMeshIndex = (int)i;
				closestTriangleIndex = (int)j;
				normalPlaneNormal = glm::cross((triangle.v1 - triangle.v0), (triangle.v2 - triangle.v0));
				normalU = u;
				normalV = v;
			}
		}
	}
	if (closestTriangleIndex < 0) // пересечений с треугольником не обнаружено -> payload.HitDistance = -1.0f;
		return Miss(ray);
	return ClosestHit(ray, hit_distance, closestMeshIndex, closestTriangleIndex, normalU, normalV, normalPlaneNormal);
}

Renderer::HitPayLoad Renderer::ClosestHit(const Ray& ray, float hitDistance, int meshIndex, int triangleIndex, float u, float v, glm::vec3 N)
{
	Renderer::HitPayLoad payload;
	payload.HitDistance = hitDistance;
	payload.meshIndex = meshIndex;
	payload.triangleIndex = triangleIndex;
	payload.u = u;
	payload.v = v;
	glm::vec3 P = ray.Origin + ray.Direction * hitDistance;
	payload.WorldPos = P;
	payload.WorldNormal = N;
	return payload;
}
Renderer::HitPayLoad Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayLoad payload;
	payload.HitDistance = -1.0f; 
	return payload;
}
