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
		//{
		//	Mesh mesh;
		//	const char* path = "src/models/cube single.obj";
		//	mesh.Mat.Albedo = glm::vec3(1.0f, .0f, .0f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}		
		//{
		//	Mesh mesh;
		//	const char* path = "src/models/trianglepyramid.obj";
		//	mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}		
		//{
		//	Mesh mesh;
		//	const char* path = "src/models/quadpyramid.obj";
		//	mesh.loadMesh(path);
		//	mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
		//	m_Scene.Meshes.push_back(mesh);
		//}
		//{
		//	Mesh mesh;
		//	const char* path = "src/models/room.obj";
		//	mesh.Mat.Albedo = glm::vec3(0.4f, .4f, .4f);
		//	mesh.loadMesh(path);
		//	m_Scene.Meshes.push_back(mesh);
		//}	
		{
			Mesh mesh;
			const char* path = "src/models/naturmonke.obj";
			mesh.loadMesh(path);
			mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
			m_Scene.Meshes.push_back(mesh);
		}
		{
			Mesh mesh;
			const char* path = "src/models/cube scene.obj";
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
		
		std::vector<Triangle> m_triangles;
		std::vector<size_t> m_mesh_sizes;
		for (size_t i = 0; i < m_Scene.Meshes.size(); ++i)
		{
			m_triangles.insert
			(
				m_triangles.end(), 
				m_Scene.Meshes[i].Triangles.begin(), 
				m_Scene.Meshes[i].Triangles.end()
			);
			m_mesh_sizes.push_back(m_Scene.Meshes[i].Triangles.size());
		}

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_triangles, m_mesh_sizes, m_Camera);

		std::cout << m_ViewportWidth << " x " << m_ViewportHeight << "\n";

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