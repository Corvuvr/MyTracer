#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include <Walnut/Random.h>
#include <Walnut/Timer.h>

#include "Camera.h"
#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"

#define TRIANGLES

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(90.0f, 0.1f, 100.0f)  
	{

#ifdef SPHERES

		// Фигуры
		{
			Sphere sphere;
			sphere.Pos = { 0.6f,1.1f,-1.9f };
			sphere.Radius = 0.3f;
			sphere.Mat.Albedo = { 1.0f, 0.0f, 0.0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Pos = { 0.0f,0.0f,-5.0f };
			sphere.Radius = 1.5f;
			sphere.Mat.Albedo = { 1.0f, 1.0f, 0.9f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}		
		
		{
			Sphere sphere;
			sphere.Pos = { -0.5f,-0.3f,-2.5f };
			sphere.Radius = 0.5f;
			sphere.Mat.Albedo = { 0.0f, 0.0f, 1.0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}

		// Большие сферы
		{
			Sphere sphere;
			sphere.Pos = { -7.0f,-77.0f,-2.0f };
			sphere.Radius = 75.0f;
			sphere.Mat.Albedo = { 0.0f, 0.0f, 1.0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Pos = { -7.0f,80.0f,-2.0f };
			sphere.Radius = 75.0f;
			sphere.Mat.Albedo = { 1.0f, 1.0f, 1.0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}



		// XYZ
		{
			Sphere sphere;
			sphere.Pos = { .0f,.0f,.0f };
			sphere.Radius = .1f;
			sphere.Mat.Albedo = { .0f, 0.0f, .0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Pos = { .1f,.0f,.0f };
			sphere.Radius = .05f;
			sphere.Mat.Albedo = { 1.0f, 0.0f, .0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Pos = { .0f,.1f,.0f };
			sphere.Radius = .05f;
			sphere.Mat.Albedo = { .0f, 1.0f, .0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Pos = { .0f,.0f,.1f };
			sphere.Radius = .05f;
			sphere.Mat.Albedo = { .0f, .0f, 1.0f };
			sphere.Mat.Roughness = 0.0f;
			m_Scene.Spheres.push_back(sphere);
		}

#endif // SPHERES

#ifdef TRIANGLES
		
		//{
		//	Mesh mesh;

		//	Triangle triangle;

		//	triangle.v0 = { 10.0f, 0.0f, 0.0f };
		//	triangle.v1 = { 0.0f, 0.0f, 10.0f };
		//	triangle.v2 = { 0.0f, 10.0f, 0.0f };
		//	mesh.Triangles.push_back(triangle);

		//	triangle.v0 = { 10.0f, 0.0f, 10.0f };
		//	triangle.v2 = { 0.0f, 0.0f, 10.0f };
		//	triangle.v1 = { 0.0f, 10.0f, 0.0f };
		//	mesh.Triangles.push_back(triangle);

		//	triangle.v0 = { 10.0f, 0.0f, 10.0f };
		//	triangle.v1 = { 0.0f, 0.0f, 10.0f };
		//	triangle.v2 = { 10.0f, 0.0f, 0.0f };
		//	mesh.Triangles.push_back(triangle);

		//	triangle.v0 = { 10.0f, 0.0f, 10.0f };
		//	triangle.v2 = { 0.0f, 10.0f, 0.0f };
		//	triangle.v1 = { 10.0f, 0.0f, 0.0f };
		//	mesh.Triangles.push_back(triangle);

		//	mesh.Mat.Albedo = { .0f, 1.0f, .0f };
		//	mesh.Mat.Roughness = 0.0f;

		//	m_Scene.Meshes.push_back(mesh);
		//}
		
		//X
		//{
		//	Triangle triangle;
		//	triangle.v0 = { 1.5f, 0.1f, 0.1f };
		//	triangle.v1 = { 1.5f, 0.0f, 0.0f };
		//	triangle.v2 = { 1.5f, 0.0f, 0.1f };
		//	Mesh mesh;
		//	mesh.Mat.Albedo = { 1.0f, .0f, .0f };
		//	mesh.Mat.Roughness = 0.0f;
		//	mesh.Triangles.push_back(triangle);
		//	m_Scene.Meshes.push_back(mesh);
		//}

#endif // TRIANGLES

		//{
		//	Mesh mesh;
		//	const char* path = "C:/Users/dpodg/Desktop/Models/Cube/cube.obj";
		//	mesh.Mat.Albedo = glm::vec3(1.0f, .0f, .0f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}		
		//{
		//	Mesh mesh;
		//	const char* path = "C:/Users/dpodg/Desktop/Models/Cone/cone.obj";
		//	mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}		
		//{
		//	Mesh mesh;
		//	const char* path = "C:/Users/dpodg/Desktop/Models/Box/box.obj";
		//	mesh.Mat.Albedo = glm::vec3(0.4f, .4f, .4f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}

		{
			Mesh mesh;
			const char* path = "C:/Users/dpodg/Desktop/Models/v1/v1.obj";
			mesh.loadMesh(path);
			mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
			m_Scene.Meshes.push_back(mesh);
		}

	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}
	virtual void OnUIRender() override
	{

#ifdef SPHERES
		ImGui::Begin("Spheres");
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);
				
				Sphere& sphere = m_Scene.Spheres[i];

				ImGui::DragFloat3("Position", glm::value_ptr(sphere.Pos), 0.1f);
				ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
				ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Mat.Albedo));
				ImGui::DragFloat("Roughness", &sphere.Mat.Roughness);
				ImGui::DragFloat("Metallic", &sphere.Mat.Metallic);
				ImGui::Spacing();
				ImGui::Spacing();

			ImGui::PopID();
		}
		ImGui::End();
#endif // SPHERES


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));	
		ImGui::Begin("Viewport");
			
			
			//ImGui::SetWindowSize("Viewport", ImVec2(320, 240));
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

			auto image = m_Renderer.GetFinalImage();

			if (image) 
				ImGui::Image(
					image->GetDescriptorSet(),
					{ (float)image->GetWidth(), (float)image->GetHeight() }, 
					ImVec2(0,1),
					ImVec2(1,0)
				);
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Settings");

			ImGui::Text("Last Render: %.3fms", m_LastRenderTime);

			

			ImGui::Text("Resolution:  %i%s%i", m_ViewportWidth,"x", m_ViewportHeight);

			ImGui::Text("Framerate:  %.1f fps", (1000.0f/m_LastRenderTime));

		ImGui::End();

		Render();
	}
private:

	Camera m_Camera;
	Renderer m_Renderer;
	Scene m_Scene;

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	uint32_t* m_ImageData = nullptr;
	float m_LastRenderTime = 0;

	void Render()
	{
		Timer timer;
		
		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);


		m_LastRenderTime = timer.ElapsedMillis();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "My Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}