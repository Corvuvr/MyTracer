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
		{
			Mesh mesh;
			const char* path = "src/models/comparison_test_1_monke.obj";
			mesh.loadMesh(path);
			mesh.Mat.Albedo = glm::vec3(1.0f, .0f, .0f);
			mesh.Mat.Roughness = 0.0f;
			m_Scene.Meshes.push_back(mesh);
		}
		{
			Mesh mesh;
			const char* path = "src/models/comparison_test_1_scene.obj";
			mesh.loadMesh(path);
			mesh.Mat.Albedo = glm::vec3(0.0f, 1.0f, .0f);
			mesh.Mat.Roughness = 0.0f;
			m_Scene.Meshes.push_back(mesh);
		}
		{
			Mesh mesh;
			const char* path = "src/models/comparison_test_1_sphere.obj";
			mesh.loadMesh(path);
			mesh.Mat.Albedo = glm::vec3(0.0f, .0f, 1.0f);
			mesh.Mat.Roughness = 0.0f;
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

			ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);

			if (ImGui::Button("Reset"))
				m_Renderer.ResetFrameIndex();

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