#pragma once

#include <vin.hpp>
#include <gin/gin.hpp>

#include <module/editor/editorwindow.hpp>
#include "graphlist.hpp"
#include "imnodes.h"

class GraphEditorWindow : public Vin::EditorWindow {
public:
	GraphEditorWindow();
	~GraphEditorWindow();

	virtual void Draw(bool* draw) final;
	virtual std::string GetName() final;

	void SetEntry(std::shared_ptr<GraphListEntry> entry);

private:
	template<typename T>
	void SetGraphPortType(size_t idx, bool input) {
		if (input)
			entry->graph->SetInput<T>(idx, T{});
		else
			entry->graph->SetOutput<T>(idx, T{});
	}

	void DrawToolWindow();

	void DrawGraphTab();
	void DrawGraphPort(size_t idx, Gin::Graph::GraphPort& port, bool input = false);
	void DrawGraphPortTab();
	void DrawProgramTab();

	void DrawAddNodeMenu(std::string path, size_t idx, std::function<size_t(Gin::Graph::Graph&, std::string)> f, ImVec2 clickPos);
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

	void Save(std::string path);
	void SaveAs();
	void Open();

	void Compile();
	void Execute(Gin::Graph::GraphContext ctx);

private:
	ImNodesContext* context;
	//std::shared_ptr<Gin::Graph::Graph> graph{};
	std::shared_ptr<GraphListEntry> entry{};
	Gin::Graph::GraphAction* focusedAction{ nullptr };
};
