#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../editor.hpp"
#include <imgui.h>
#include <imgui_node_editor.h>
#include <gin/graph/graph.hpp>
#include <gin/graph/serialization/graphserdes.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../stringfmt.hpp"
#include "../nodebuilder.hpp"

struct GraphEntry {
    std::string path{ "" };
    std::string name{ "<new graph>" };
    std::shared_ptr<Gin::Graph::Graph> graph{};
    std::vector<ImVec2> positions{}; //Editor & Nodes position;
};

enum class MeshBuilderType {
    MarchingCube,
    SurfaceNet
};

class GraphEditorWindow : public EditorWindow {
public:
    ~GraphEditorWindow();

    void Initialize() final;
    void Draw(bool* open) final;
    std::string_view GetName() final;
    void OnEvent(EventHandler& handler) final;
    GraphEntry& GetCurrentGraph();

private:
    template<typename T>
    void SetGraphPortType(size_t idx, bool input) {
        if (input)
            graphs[currentGraphIdx].graph->SetInput<T>(idx, T{});
        else
            graphs[currentGraphIdx].graph->SetOutput<T>(idx, T{});
    }

    void SetGraphEntry(uint32_t i);
    void Save(std::string_view path);
    void SaveAs();
    void Open();

    void DrawAddNodeMenu(std::string path, size_t idx, std::function<size_t(Gin::Graph::Graph&, std::string)>& f, ImVec2 clickPos);
    void DrawAddNodePopup();

    void DrawGraphPortTypeList(size_t idx, Gin::Graph::GraphPort& port, bool input);

    void DrawToolWindow();
    void DrawGraphToolTab();
    void DrawGraphPortToolTab();
    void DrawGraphProgramToolTab();
    void DrawMeshBuilderToolTab();

    void DrawPin(Gin::Graph::Port& port, Gin::Graph::GraphId id, bool input, bool hasLink = false);
    void DrawGraphPortNode();
    void DrawNode(Gin::Graph::GraphNodeOperator<Gin::Graph::Node>& node);
    void DrawLinks();

    void HandleLinkCreation();
    void HandleDeletion();

    void BuildVolume(Gin::Graph::GraphContext& context);

private:
    std::vector<GraphEntry> graphs{};
    uint32_t currentGraphIdx{ 0 };
    Gin::Graph::GraphAction* focusedAction{ nullptr };
    ax::NodeEditor::EditorContext* context{ nullptr };

    Gin::Thread::ThreadPool threadPool{};
    MeshBuilderType meshBuilderType{ MeshBuilderType::SurfaceNet };
    bool recalculateNormal{ true };
};