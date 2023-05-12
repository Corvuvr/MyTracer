#pragma once
#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"


#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "CL/cl.h"
#include <vector>

#define NVIDIA
#define MAX_SOURCE_SIZE (0x100000)
class Renderer
{
public:
	//Renderer() = default;
	Renderer()
	{
#ifdef AMD
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
		std::ifstream helloWorldKernel("src/kernels/" + (std::string)m_file_ch + ".cl");
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
#endif // AMD

#ifdef NVIDIA
		cl_int err = 0;
		const char* m_file_ch = "R1";

		cl_platform_id platform;
		cl_uint num_devices;
		clGetPlatformIDs(1, &platform, &num_devices);

		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);

		cl_context context = clCreateContext(0, num_devices, &device, NULL, NULL, &err);
		this->context = context;

		//Читаем kernel-код
		char* sourcepath = "src/kernels/R1.cl";
		FILE* fp = fopen(sourcepath, "r");
		if (!fp) 
		{
			fprintf(stderr, "Failed to open kernel file '%s': %s\n", sourcepath, strerror(errno));
		}

		char* source_str = (char*)malloc(MAX_SOURCE_SIZE);
		size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);

		cl_program program = clCreateProgramWithSource(context,
			1,
			(const char**)&source_str,
			&source_size, &err);

		this->program = program;
		err = clBuildProgram(this->program, num_devices, &device, NULL, NULL, NULL);

		cl_kernel kernel = clCreateKernel(this->program, m_file_ch, &err);
		this->kernel = kernel;

		cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);
		this->queue = queue;
#endif // NVIDIA
	}
	//void Render(const Scene& scene, const Camera& camera);
	void Render( std::vector<Triangle> &triangles, const std::vector<size_t>& mesh_sizes, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	//struct HitPayLoad
	//{
	//	float HitDistance;
	//	float u = 0, v = 0;
	//	int meshIndex, triangleIndex;
	//	glm::vec3 WorldPos;
	//	glm::vec3 WorldNormal;
	//};

private:
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	std::vector<uint32_t> m_ImageHorisontalIter, m_ImageVerticalIter;

#ifdef AMD
	cl::Program			program;
	cl::Context			context;
	cl::Device			device;
	cl::CommandQueue	queue;
	cl::Kernel			kernel;
#endif // AMD
#ifdef NVIDIA
	cl_program			program;
	cl_context			context;
	cl_device_id		device;
	cl_command_queue	queue;
	cl_kernel			kernel;
#endif // NVIDIA
	size_t				work_group = 256;
};
