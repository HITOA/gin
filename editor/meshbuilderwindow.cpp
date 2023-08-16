#include "meshbuilderwindow.hpp"
#include <imgui.h>
#include <gin/spatial/sampler.hpp>
#include <gin/mesh/marchingcube.hpp>

MeshBuilderWindow::MeshBuilderWindow()
{
	mesh = std::make_shared<Vin::DynamicMesh>(Vin::VertexBufferLayout{
			   Vin::VertexBufferElement{ Vin::VertexAttribute::Position, Vin::VertexAttributeType::Float3 },
			   Vin::VertexBufferElement{ Vin::VertexAttribute::Normal, Vin::VertexAttributeType::Float3 } }, Vin::BufferIndexType::UnsignedInt32);
}

MeshBuilderWindow::~MeshBuilderWindow()
{
}

void MeshBuilderWindow::Draw(bool* draw)
{
	if (ImGui::Begin("Mesh Builder", draw)) {

		ImGui::BeginTabBar("Mesh Builder Tool Window Tab Bar");

		if (ImGui::BeginTabItem("HeightMap")) {
			DrawHeightmapTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Volume")) {
			DrawVolumeTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();

		ImGui::End();
	}
}

std::string MeshBuilderWindow::GetName()
{
	return "Mesh Builder";
}

void MeshBuilderWindow::SetGraph(std::shared_ptr<Gin::Graph::Graph> graph)
{
	this->graph = graph;
}

void MeshBuilderWindow::SetScene(std::shared_ptr<Vin::Scene> scene)
{
	this->scene = scene;
	(*scene)->CreateEntity(Vin::Transform<float>{Vin::Vector3<float>{0.0, -1.0, 0.0}}, Vin::MeshRenderer{ mesh.get() });
}

void MeshBuilderWindow::SetMaterial(std::shared_ptr<Vin::Material> material)
{
	this->material = material;
}

void MeshBuilderWindow::DrawHeightmapTab()
{
}

void MeshBuilderWindow::DrawVolumeTab()
{
	static Gin::Graph::GraphContext ctx{};

	ImGui::InputFloat("Scale", &ctx.scale);

	ImGui::PushItemWidth(75);

	ImGui::InputDouble("##Origin_X", &ctx.bounds.origin.x());
	ImGui::SameLine();
	ImGui::InputDouble("##Origin_Y", &ctx.bounds.origin.y());
	ImGui::SameLine();
	ImGui::InputDouble("##Origin_Z", &ctx.bounds.origin.z());
	ImGui::SameLine();
	ImGui::TextUnformatted("Origin");

	ImGui::InputDouble("##Extent_X", &ctx.bounds.extent.x());
	ImGui::SameLine();
	ImGui::InputDouble("##Extent_Y", &ctx.bounds.extent.y());
	ImGui::SameLine();
	ImGui::InputDouble("##Extent_Z", &ctx.bounds.extent.z());
	ImGui::SameLine();
	ImGui::TextUnformatted("Extent");

	ImGui::PopItemWidth();

	if (ImGui::Button("Build")) {
		BuildVolume(ctx);
	}
}

void MeshBuilderWindow::BuildVolume(Gin::Graph::GraphContext ctx)
{
	size_t volumeIdx = graph->HasOutput("_Volume");

	if (volumeIdx == std::numeric_limits<size_t>::max()) {
		Vin::Logger::Err("Can't build volume : a Spatial<Float> _Volume output is required.");
		return;
	}

	Gin::Graph::GraphPort& volumePort = graph->GetOutputPort(volumeIdx);

	if (volumePort.GetType().type != (Gin::Graph::PortType)((int)Gin::Graph::PortType::Number + (int)Gin::Graph::PortType::Spatial)) {
		Vin::Logger::Err("Can't build volume : a Spatial<Float> _Volume output is required.");
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> start{};

	start = std::chrono::system_clock::now();

	Gin::Thread::ThreadPool pool{};

	try {
		graph->Execute(ctx, pool);
	}
	catch (std::exception& e) {
		Vin::Logger::Err("Graph Execution Error : {}", e.what());
	}

	std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
	Vin::Logger::Log("Graph Execution Took : {}ms", elapsed.count() * 1000.0);

	Gin::Spatial::Spatial<float> spatial = *(Gin::Spatial::Spatial<float>*)volumePort.GetProperty();
	Gin::Spatial::Sampler<float> sampler{ spatial };

	sampler.SetScale(ctx.scale);
	sampler.SetBounds(ctx.bounds);

	Gin::Mesh::IndexedMesh indexedMesh{};
	Gin::Mesh::MarchingCubeMeshBuilder builder{};
	builder.bounds = ctx.bounds;
	builder.scale = ctx.scale;

	start = std::chrono::system_clock::now();

	try {
		builder.Build(indexedMesh, sampler);
		indexedMesh.RecalculateNormal();
	}
	catch (std::exception& e) {
		Vin::Logger::Err("Mesh Builder Execution Error : {}", e.what());
	}

	elapsed = std::chrono::system_clock::now() - start;
	Vin::Logger::Log("Mesh Builder Execution Took : {}ms", elapsed.count() * 1000.0);

	Vin::Logger::Log("Vertex Count : {}", indexedMesh.GetVertexBufferSize());
	Vin::Logger::Log("Indices Count : {}", indexedMesh.GetIndexBufferSize());

	mesh->SetVertexData(indexedMesh.GetVertexBuffer(), indexedMesh.GetVertexBufferSize());
	mesh->SetIndexData(indexedMesh.GetIndexBuffer(), indexedMesh.GetIndexBufferSize());
	mesh->SetMaterialCount(1);
	mesh->SetMaterial(material, 0);
}
