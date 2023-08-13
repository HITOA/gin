#pragma once

#include <vin.hpp>
#include <module/editor/editorwindow.hpp>
#include <gin/graph/graph.hpp>

class MeshBuilderWindow : public Vin::EditorWindow {
public:
	MeshBuilderWindow();
	~MeshBuilderWindow();

	virtual void Draw(bool* draw) final;
	virtual std::string GetName() final;

	void SetGraph(std::shared_ptr<Gin::Graph::Graph> graph);
	void SetScene(std::shared_ptr<Vin::Scene> scene);
	void SetMaterial(std::shared_ptr<Vin::Material> material);

private:
	void DrawHeightmapTab();
	void DrawVolumeTab();

	void BuildVolume(Gin::Graph::GraphContext ctx);

private:
	std::shared_ptr<Gin::Graph::Graph> graph{};
	std::shared_ptr<Vin::Scene> scene{};
	std::shared_ptr<Vin::Material> material{};
	std::shared_ptr<Vin::DynamicMesh> mesh{};
};
