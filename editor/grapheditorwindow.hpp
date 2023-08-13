#pragma once

#include <vin.hpp>
#include <gin/gin.hpp>

#include <module/editor/editorwindow.hpp>
#include "imnodes.h"

class GraphEditorWindow : public Vin::EditorWindow {
public:
	GraphEditorWindow();
	~GraphEditorWindow();

	virtual void Draw(bool* draw) final;
	virtual std::string GetName() final;

	void SetGraph(std::shared_ptr<Gin::Graph::Graph> graph);

private:
	template<typename T>
	void SetGraphPortType(size_t idx, bool input) {
		if (input)
			graph->SetInput<T>(idx, T{});
		else
			graph->SetOutput<T>(idx, T{});
	}

	void DrawToolWindow();

	void DrawGraphPort(size_t idx, Gin::Graph::GraphPort& port, bool input = false);
	void DrawGraphPortTab();
	void DrawProgramTab();

	void DrawAddNodeMenu(std::string path, size_t idx, std::function<size_t(Gin::Graph::Graph&)> f, ImVec2 clickPos);
	void DrawAddNodePopup();
	void SetPinColor(Gin::Graph::Port& port);
	void DrawPin(Gin::Graph::Port& port, int idx, bool input);
	void DrawPin(Gin::Graph::GraphPort& port, int idx, bool input);
	void DrawGraphPortNode();
	void DrawNode(Gin::Graph::GraphNodeOperator<Gin::Graph::Node>& node);
	void DrawLink();
	void HandleLinkCreation();
	void HandleLinkDeletion();
	void HandleNodeDeletion();

	void DeleteLinkById(int id);
	void DeleteNodeById(int id);
	void DeleteGraphInputByIdx(size_t id);
	void DeleteGraphOutputByIdx(size_t id);

	void Compile();
	void Execute(Gin::Graph::GraphContext ctx);

private:
	ImNodesContext* context;
	std::shared_ptr<Gin::Graph::Graph> graph{};
	std::vector<std::pair<size_t, size_t>> links{};
	Gin::Graph::GraphAction* focusedAction{ nullptr };
};
