#include "meshbuilderwindow.hpp"
#include <imgui.h>

MeshBuilderWindow::MeshBuilderWindow()
{
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
	if (!graph->HasOutput("_Volume")) {
		Vin::Logger::Err("Can't build volume : a 3D field _Volume output is required.");
	//	return;
	}



	std::chrono::time_point<std::chrono::system_clock> start{};

	start = std::chrono::system_clock::now();

	try {
		graph->Execute(ctx);
	}
	catch (std::exception& e) {
		Vin::Logger::Err("Graph Execution Error : {}", e.what());
	}

	std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
	Vin::Logger::Log("Graph Execution Took : {}ms", elapsed.count() * 1000.0);
}
