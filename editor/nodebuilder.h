#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_node_editor.h>

class NodeBuilder {
public:
    void Begin(ax::NodeEditor::NodeId id);
    void End();
    void BeginHeader();
    void EndHeader();
    void BeginContent();
    void EndContent();
    void BeginInput();
    void EndInput();
    void BeginOutput();
    void EndOutput();

private:
    ImVec2 headerMin{};
    ImVec2 headerMax{};
    ax::NodeEditor::NodeId nodeId{};
};