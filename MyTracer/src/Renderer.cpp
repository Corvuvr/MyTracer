#include <execution>
#include <fstream>
#include <iostream>
#include "Renderer.h"
#include "Walnut/Random.h"
#include "PointLight.h"

#define NVIDIA

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
	static uint32_t ConvertToRGBA_cl(const cl_float4 color)
	{
		uint8_t r = (uint8_t)(color.s[0] * 255.0f);
		uint8_t g = (uint8_t)(color.s[1] * 255.0f);
		uint8_t b = (uint8_t)(color.s[2] * 255.0f);
		uint8_t a = (uint8_t)(color.s[3] * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}
	glm::vec3 from_cl_to_glm(cl_float3 vec)
	{
		return glm::vec3( vec.x, vec.y,vec.z );
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

	m_ImageHorisontalIter.resize(width);
	m_ImageVerticalIter.resize(height);

	for (uint32_t i = 0; i < width; ++i)
		m_ImageHorisontalIter[i] = i;

	for (uint32_t i = 0; i < height; ++i)
		m_ImageVerticalIter[i] = i;
}

void Renderer::Render( std::vector<struct Triangle>& triangles_X, const std::vector<size_t>& mesh_sizes, const Camera& camera)
{

	// prep-фаза ====================================================================================================================
	m_ActiveCamera = &camera;

	cl_int					err					=  0;
	float					gpu_debug_entity	= -1;									//KERNEL ARG
	std::vector<float>		gpu_debug_entities	= { -1, -1, -1, -1, -1, -1, -1, -1, -1 };			//KERNEL ARG

	glm::vec3				pos					= m_ActiveCamera->GetPosition();
	cl_float3				ray_origin			= { pos.x, pos.y, pos.z };				//KERNEL ARG
	std::vector<cl_float3>	ray_directions		= m_ActiveCamera->GetClRayDirections();	//KERNEL ARG

	
	size_t					work_size			= camera.GetResolution(); //ray_directions.size();
	const size_t			NDRange				= work_size + work_group - (work_size % work_group);

	//std::vector<struct Triangle> triangles		= triangles_X;							//KERNEL ARG
	
	uint32_t*				temp_Image_Data		= new uint32_t[work_size];				//KERNEL ARG
	uint32_t				triangles_size		= triangles_X.size();						//KERNEL ARG
	
	
	// kernel-фаза ==================================================================================================================
	
#ifdef AMD
	cl::Buffer triangles_buff(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(struct Triangle) * triangles.size(),
		triangles.data(), 
		&err
	);
	cl::Buffer triangles_size_buff
	(
		context,
		CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
		sizeof(uint32_t),
		&triangles_size,
		&err
	);
	cl::Buffer ray_directions_buff
	(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_float3) * ray_directions.size(),
		ray_directions.data(),
		&err
	);
	cl::Buffer ray_origin_buff
	(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_float3),
		&ray_origin,
		&err
	);
	cl::Buffer output_color_buff
	(
		context,
		CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,
		sizeof(*temp_Image_Data) * work_size,
		nullptr,
		&err
	);
	cl::Buffer dot_buff(
		context,
		CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
		sizeof(float) * gpu_debug_entities.size(),
		gpu_debug_entities.data(),
		&err
	);


	err = kernel.setArg(0, triangles_buff);
	err = kernel.setArg(1, triangles_size_buff);
	err = kernel.setArg(2, ray_directions_buff);
	err = kernel.setArg(3, ray_origin_buff);
	err = kernel.setArg(4, output_color_buff);
	err = kernel.setArg(5, dot_buff);

	err = queue.enqueueNDRangeKernel(
		kernel,
		0,
		cl::NDRange(work_size)
	);
	err = queue.enqueueReadBuffer(
		dot_buff,
		CL_TRUE,
		0,
		sizeof(float) * gpu_debug_entities.size(),
		gpu_debug_entities.data()
	);
	err = queue.enqueueReadBuffer(
		output_color_buff,
		CL_TRUE,
		0,
		sizeof(*temp_Image_Data) * work_size,
		temp_Image_Data
	);
	cl::finish();
#endif // AMD
	
	
	
	

	cl_mem triangles_buff = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(struct Triangle) * triangles_X.size(),
		triangles_X.data(),
		&err
	);

	cl_mem triangles_size_buff = clCreateBuffer(
		context,
		CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
		sizeof(uint32_t),
		&triangles_size,
		&err
	);

	cl_mem ray_directions_buff = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_float3)* ray_directions.size(),
		ray_directions.data(),
		&err
	);

	cl_mem ray_origin_buff = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_float3),
		&ray_origin,
		&err
	);

	cl_mem output_color_buff = clCreateBuffer(
		context,
		CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,
		sizeof(*temp_Image_Data)* work_size,
		nullptr,
		&err
	);

	cl_mem dot_buff = clCreateBuffer(
		context,
		CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
		sizeof(float)* gpu_debug_entities.size(),
		gpu_debug_entities.data(),
		&err
	);

	err = clSetKernelArg(kernel, 0, sizeof(triangles_buff), &triangles_buff);
	err = clSetKernelArg(kernel, 1, sizeof(triangles_size_buff), &triangles_size_buff);
	err = clSetKernelArg(kernel, 2, sizeof(ray_directions_buff), &ray_directions_buff);
	err = clSetKernelArg(kernel, 3, sizeof(ray_origin_buff), &ray_origin_buff);
	err = clSetKernelArg(kernel, 4, sizeof(output_color_buff), &output_color_buff);
	err = clSetKernelArg(kernel, 5, sizeof(dot_buff), &dot_buff);
	
	err = clEnqueueNDRangeKernel(
		queue,
		kernel,
		1,
		NULL,
		&NDRange, 0, NULL, NULL, NULL
	);
	err = clEnqueueReadBuffer(
		queue, 
		dot_buff,
		CL_TRUE,
		0,
		sizeof(float) * gpu_debug_entities.size(),
		gpu_debug_entities.data(),
		NULL, NULL, NULL
	);
	err = clEnqueueReadBuffer(
		queue,
		output_color_buff,
		CL_TRUE,
		0,
		sizeof(*temp_Image_Data) * work_size,
		temp_Image_Data,
		NULL, NULL, NULL
	);

	clFinish(queue);

	// payload-фаза ==================================================================================================================
	
	clReleaseMemObject( triangles_buff);
	clReleaseMemObject(triangles_size_buff);
	clReleaseMemObject(ray_directions_buff);
	clReleaseMemObject(ray_origin_buff);
	clReleaseMemObject(output_color_buff);
	clReleaseMemObject(dot_buff);


	m_FinalImage->SetData(temp_Image_Data);
	delete[] temp_Image_Data;
	
}


