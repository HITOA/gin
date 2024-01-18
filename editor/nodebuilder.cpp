#include "nodebuilder.h"

void NodeBuilder::Begin(ax::NodeEditor::NodeId id) {
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodePadding, ImVec4(8, 4, 8, 8));
    ax::NodeEditor::BeginNode(id);
    ImGui::PushID(id.AsPointer());
    ImGui::BeginVertical("Node");
    nodeId = id;
}

void NodeBuilder::End() {
    ax::NodeEditor::PopStyleVar();
    ImGui::EndVertical();
    ImGui::PopID();
    ax::NodeEditor::EndNode();

    if (ImGui::IsItemVisible()) {
        ImDrawList* drawList = ax::NodeEditor::GetNodeBackgroundDrawList(nodeId);
        float halfBorderWidth = ax::NodeEditor::GetStyle().NodeBorderWidth * 0.5f;

        if ((headerMax.x > headerMin.x) && (headerMax.y > headerMin.y)) {
            drawList->AddRectFilled(headerMin - ImVec2(8 - halfBorderWidth, 4 - halfBorderWidth),
                                    headerMax + ImVec2(8 - halfBorderWidth, 0),
                                    ImColor{ 0.5f, 0.15f, 0.2f, 1.0f },
                                    ax::NodeEditor::GetStyle().NodeRounding, ImDrawFlags_RoundCornersTop);
        }
    }
}

void NodeBuilder::BeginHeader() {
    ImGui::BeginHorizontal("Header");
}

void NodeBuilder::EndHeader() {
    ImGui::EndHorizontal();
    headerMin = ImGui::GetItemRectMin();
    headerMax = ImGui::GetItemRectMax();
    ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.y * 2.0f);
}

void NodeBuilder::BeginContent() {
    ImGui::BeginHorizontal("Content");
    ImGui::Spring(0, 0);
}

void NodeBuilder::EndContent() {
    ImGui::EndHorizontal();
}

void NodeBuilder::BeginInput() {
    ImGui::BeginVertical("Input", ImVec2(0, 0), 0.0f);
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PivotAlignment, ImVec2(0, 0.5f));
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PivotSize, ImVec2(0, 0));
}

void NodeBuilder::EndInput() {
    ImGui::Spring(1, 0);
    ax::NodeEditor::PopStyleVar(2);
    ImGui::EndVertical();
}

void NodeBuilder::BeginOutput() {
    ImGui::Spring(1);
    ImGui::BeginVertical("Output", ImVec2(0, 0), 0.0f);
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PivotSize, ImVec2(0, 0));
}

void NodeBuilder::EndOutput() {
    ImGui::Spring(1, 0);
    ax::NodeEditor::PopStyleVar(2);
    ImGui::EndVertical();
}