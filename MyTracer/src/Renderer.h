#pragma once
#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "Scene.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

#include "CL/cl.hpp"
#include <vector>
class Renderer
{

public:
	//Renderer() = default;
	Renderer()
	{
		//std::string m_file = "RenderTest1";						// называть кернел также как .cl-файл
		const char* m_file_ch = "R1";						// называть кернел также как .cl-файл

		//Определяем платформу
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		auto platform = platforms.front();

		//Определяем GPU
		std::vector < cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		auto device = devices.front();
		this->device = device;

		cl::Context context(device);
		this->context = context;

		//Читаем kernel-код
		std::ifstream helloWorldKernel("src/kernels/"+ (std::string)m_file_ch +".cl");
		if (helloWorldKernel.is_open())
		{
			std::string src
			(
				std::istreambuf_iterator<char>(helloWorldKernel),
				(std::istreambuf_iterator<char>())
			);
			cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

			cl::Program program(context, sources);
			program.build("-cl-std=CL1.2");
			this->program = program;
			
			cl::Kernel kernel(this->program, m_file_ch);
			//cl::Kernel kernel(this->program, m_file);
			this->kernel = kernel;
			cl::CommandQueue queue(context, device);
			this->queue = queue;	
		}
		else
		{
			std::cout << "file is not opened\n";
		}

	}
	//void Render(const Scene& scene, const Camera& camera);
	void Render(const std::vector<Triangle> &triangles, const std::vector<size_t>& mesh_sizes, const Camera& camera);
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

	//glm::vec4 PerPixel(uint32_t x, uint32_t y); //  ray gen
	
	//HitPayLoad TraceRay(const Ray& ray);
	//HitPayLoad ClosestHit(const Ray& ray, float hitDistance, int meshIndex, int triangleIndex, float u, float v, glm::vec3 N);
	//HitPayLoad Miss(const Ray& ray);

private:
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	std::vector<uint32_t> m_ImageHorisontalIter, m_ImageVerticalIter;

	cl::Program			program;
	cl::Context			context;
	cl::Device			device;
	cl::CommandQueue	queue;
	cl::Kernel			kernel;
	size_t				work_group = 256;
};
