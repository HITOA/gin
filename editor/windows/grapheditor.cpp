#include "grapheditor.hpp"
#include <misc/cpp/imgui_stdlib.h>
#include <nfd.h>
#include <gin/module/registry.hpp>
#include <iostream>
#include <gin/mesh/indexedmesh.hpp>
#include <gin/mesh/marchingcube.hpp>
#include <gin/mesh/surfacenet.hpp>
#include <gin/field/sampler.hpp>
#include "view.hpp"
#include "profiler.hpp"

GraphEditorWindow::~GraphEditorWindow() noexcept {
    ax::NodeEditor::DestroyEditor(context);
}

void GraphEditorWindow::Initialize() {
    ax::NodeEditor::Config config{};
    config.NavigateButtonIndex = 2;
    context = ax::NodeEditor::CreateEditor(&config);

    GraphEntry defaultEntry{};
    defaultEntry.graph = std::make_shared<Gin::Graph::Graph>();
    graphs.push_back(defaultEntry);
}

void GraphEditorWindow::Draw(bool *open) {
    if (ImGui::Begin("Graph Editor", open)) {

        ImGui::Columns(2);

        DrawToolWindow();
        ImGui::NextColumn();

        ax::NodeEditor::SetCurrentEditor(context);
        ax::NodeEditor::Begin("My Editor", ImVec2(0.0, 0.0f));

        ax::NodeEditor::Suspend();
        if (ax::NodeEditor::ShowBackgroundContextMenu() ||
                (ImGui::IsKeyPressed(ImGuiKey_Space) && ImGui::IsItemHovered()))
            ImGui::OpenPopup("Add Node");

        DrawAddNodePopup();
        ax::NodeEditor::Resume();

        DrawGraphPortNode();
        for (size_t i = 0; i < graphs[currentGraphIdx].graph->GetNodeCount(); ++i) {
            auto node = graphs[currentGraphIdx].graph->GetNode<Gin::Graph::Node>(i);
            DrawNode(node);
        }
        DrawLinks();

        HandleLinkCreation();
        HandleDeletion();

        ax::NodeEditor::End();

        ImGui::Columns();
    }
    ImGui::End();
}

std::string_view GraphEditorWindow::GetName() {
    return "Graph Editor";
}

void GraphEditorWindow::SetGraphEntry(uint32_t i) {
    {
        GraphEntry& entry = graphs[currentGraphIdx];
        entry.positions.clear();
        entry.positions.push_back(ax::NodeEditor::GetNodePosition(GRAPH_INPUT_NODE_ID));
        entry.positions.push_back(ax::NodeEditor::GetNodePosition(GRAPH_OUTPUT_NODE_ID));

        for (size_t i = 0; i < entry.graph->GetNodeCount(); ++i)
            entry.positions.push_back(ax::NodeEditor::GetNodePosition(i + 1));
    }

    currentGraphIdx = i;

    if (graphs[currentGraphIdx].positions.size() >= 2) {
        GraphEntry& entry = graphs[currentGraphIdx];
        ax::NodeEditor::SetNodePosition(GRAPH_INPUT_NODE_ID, entry.positions[0]);
        ax::NodeEditor::SetNodePosition(GRAPH_OUTPUT_NODE_ID, entry.positions[1]);

        for (size_t i = 2; i < entry.positions.size(); ++i)
            ax::NodeEditor::SetNodePosition(i - 1, entry.positions[i]);
    }
}

void GraphEditorWindow::Save(std::string_view path) {
    graphs[currentGraphIdx].path = path;

    Gin::Graph::Serialization::SerializedGraph serializedGraph{};
    Gin::Graph::Serialization::SerializeGraph(*graphs[currentGraphIdx].graph.get(), serializedGraph);

    {
        ImVec2 inp = ax::NodeEditor::GetNodePosition(GRAPH_INPUT_NODE_ID);
        ImVec2 onp = ax::NodeEditor::GetNodePosition(GRAPH_OUTPUT_NODE_ID);

        serializedGraph.graphData["editor"]["inputs"]["position"][0] = inp.x;
        serializedGraph.graphData["editor"]["inputs"]["position"][1] = inp.y;

        serializedGraph.graphData["editor"]["outputs"]["position"][0] = onp.x;
        serializedGraph.graphData["editor"]["outputs"]["position"][1] = onp.y;
    }

    for (size_t i = 0; i < serializedGraph.nodesData.size(); ++i) {
        ImVec2 cnp = ax::NodeEditor::GetNodePosition(i + 1);

        serializedGraph.nodesData[i]["editor"]["position"][0] = cnp.x;
        serializedGraph.nodesData[i]["editor"]["position"][1] = cnp.y;
    }

    serializedGraph.graphName = graphs[currentGraphIdx].name;

    Gin::Graph::Serialization::SaveSerializedGraphToFile(serializedGraph, std::string{ path });
}

void GraphEditorWindow::SaveAs() {
    static nfdfilteritem_t filterItem[1] = { { "Gin Graph", "gg" } };
    nfdchar_t* outPath;
    nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, NULL, "newgraph");
    if (result == NFD_OKAY)
    {
        std::string path{ outPath };
        Save(path);
        NFD_FreePath(outPath);
    }
}

void GraphEditorWindow::Open() {
    static nfdfilteritem_t filterItem[1] = { { "Gin Graph", "gg" } };
    nfdchar_t* outPath;
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result != NFD_OKAY)
    {
        return;
        NFD_FreePath(outPath);
    }

    std::string path{ outPath };

    Gin::Graph::Serialization::SerializedGraph serializedGraph{};
    Gin::Graph::Serialization::LoadSerializedGraphFromFile(serializedGraph, path);

    GraphEntry entry{};
    entry.graph = std::make_shared<Gin::Graph::Graph>();
    Gin::Graph::Serialization::DeserializeGraph(*entry.graph, serializedGraph);
    entry.name = serializedGraph.graphName;
    entry.path = path;

    if (serializedGraph.graphData.contains("editor")) {
        entry.positions.emplace_back(serializedGraph.graphData["editor"]["inputs"]["position"][0], serializedGraph.graphData["editor"]["inputs"]["position"][1]);
        entry.positions.emplace_back(serializedGraph.graphData["editor"]["outputs"]["position"][0], serializedGraph.graphData["editor"]["outputs"]["position"][1]);

        for (size_t i = 0; i < serializedGraph.nodesData.size(); ++i)
            entry.positions.emplace_back(serializedGraph.nodesData[i]["editor"]["position"][0], serializedGraph.nodesData[i]["editor"]["position"][1]);
    }

    graphs.push_back(entry);
    SetGraphEntry(graphs.size() - 1);

    NFD_FreePath(outPath);
}

void GraphEditorWindow::DrawAddNodeMenu(std::string path, size_t idx,
                                        std::function<size_t(Gin::Graph::Graph &, std::string)>& f, ImVec2 clickPos) {
    size_t i = idx;
    do {
        ++i;

        if (i >= path.size()) {
            if (ImGui::MenuItem(path.substr(idx, i - idx).c_str())) {
                ax::NodeEditor::SetNodePosition(f(*graphs[currentGraphIdx].graph, std::string{ path }) + 1, ax::NodeEditor::ScreenToCanvas(clickPos));
            }
            return;
        }
    } while (path[i] != '/');

    if (ImGui::BeginMenu(path.substr(idx, i - idx).c_str())) {
        DrawAddNodeMenu(path, i + 1, f, clickPos);
        ImGui::EndMenu();
    }
}

void GraphEditorWindow::DrawAddNodePopup() {
    static std::string filter{};

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
    if (ImGui::BeginPopup("Add Node")) {
        const ImVec2 clickPos = ImGui::GetMousePosOnOpeningCurrentPopup();

        if (ImGui::IsWindowAppearing())
            ImGui::SetKeyboardFocusHere(0);
        ImGui::InputText("##filter", &filter);
        ImGui::Separator();

        for (auto& entry : Gin::Module::GetNodeRegistry()) {
            size_t i = entry.first.size();
            do { --i; } while (entry.first[i] != '/');
            std::string name{ entry.first.substr(i + 1) };

            if (filter.empty()) {
                DrawAddNodeMenu(entry.first, 0, entry.second, clickPos);
            } else if (name.find(filter) != std::string::npos) {
                if (ImGui::MenuItem(name.c_str())) {
                    ax::NodeEditor::SetNodePosition(entry.second(*graphs[currentGraphIdx].graph, entry.first) + 1, ax::NodeEditor::ScreenToCanvas(clickPos));
                }
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void GraphEditorWindow::DrawGraphPortTypeList(size_t idx, Gin::Graph::GraphPort &port, bool input) {
    std::string name{};

    if (input)
        name = "##Graph_Input(" + std::to_string(idx) + ")";
    else
        name = "##Graph_Output(" + std::to_string(idx) + ")";

    if (ImGui::BeginListBox(name.c_str(), ImVec2{ 0, 100 })) {

        ImGui::TextUnformatted("Name : ");
        ImGui::SameLine();
        ImGui::InputText((name + "_Name").c_str(), &port.GetName());

        ImGui::TextUnformatted("Type : ");
        ImGui::SameLine();
        if (ImGui::BeginCombo((name + "_Type").c_str(), port.GetType().fullName)) {
            if (ImGui::Selectable("Float"))
                SetGraphPortType<Gin::Field::Sampler<Gin::Math::Scalar>>(idx, input);
            if (ImGui::Selectable("Vector2"))
                SetGraphPortType<Gin::Field::Sampler<Gin::Math::Vector2>>(idx, input);
            if (ImGui::Selectable("Vector3"))
                SetGraphPortType<Gin::Field::Sampler<Gin::Math::Vector3>>(idx, input);
            if (ImGui::Selectable("Vector4"))
                SetGraphPortType<Gin::Field::Sampler<Gin::Math::Vector4>>(idx, input);

            ImGui::EndCombo();
        }

        if (input) {
            switch (port.GetType().type) {
                case (Gin::Graph::PortType)34:
                {
                    Gin::Field::Sampler<Gin::Math::Scalar>* property = (Gin::Field::Sampler<Gin::Math::Scalar>*)port.GetProperty();
                    if (property->IsFieldOfType<Gin::Field::ConstantField<Gin::Math::Scalar>>()) {
                        ImGui::InputFloat("##GraphInputValue", &(property->GetField<Gin::Field::ConstantField<Gin::Math::Scalar>>()->Get()));
                    }
                    break;
                }
                default:
                    break;
            }
        }

        if (ImGui::Button("Delete")) {
            if (input) {
                graphs[currentGraphIdx].graph->RemoveInput(idx);
            }
            else {
                graphs[currentGraphIdx].graph->RemoveOutput(idx);
            }
        }

        ImGui::EndListBox();
    }
}

void GraphEditorWindow::DrawToolWindow() {
    if (ImGui::BeginChild("Graph Editor Tool Window")) {

        ImGui::BeginTabBar("Graph Editor Tool Window Tab Bar");

        if (ImGui::BeginTabItem("Graph")) {
            DrawGraphToolTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Graph Port")) {
            DrawGraphPortToolTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Program")) {
            DrawGraphProgramToolTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Mesh Builder")) {
            DrawMeshBuilderToolTab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::EndChild();
}

void GraphEditorWindow::DrawGraphToolTab() {
    if (ImGui::BeginCombo("##Graph_List", graphs[currentGraphIdx].name.c_str())) {
        for (size_t i = 0; i < graphs.size(); ++i) {
            std::string label = graphs[i].name + "##" + std::to_string(i);
            if (ImGui::Selectable(label.c_str())) {
                SetGraphEntry(i);
            }
        }

        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::Button("+")) {
        GraphEntry entry{};
        entry.graph = std::make_shared<Gin::Graph::Graph>();
        graphs.push_back(entry);
        SetGraphEntry(graphs.size() - 1);
    }

    ImGui::TextUnformatted("Name : ");
    ImGui::SameLine();
    ImGui::InputText("##GraphName", &graphs[currentGraphIdx].name);

    if (ImGui::Button("Save..")) {
        if (graphs[currentGraphIdx].path.empty()) {
            SaveAs();
        }
        else {
            Save(graphs[currentGraphIdx].path);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Save As..")) {
        SaveAs();
    }

    ImGui::SameLine();

    if (ImGui::Button("Open..")) {
        Open();
    }
}

void GraphEditorWindow::DrawGraphPortToolTab() {
    ImGui::TextUnformatted("Inputs : ");

    GraphEntry& entry = graphs[currentGraphIdx];

    for (size_t i = 0; i < entry.graph->GetInputsCount(); ++i)
        DrawGraphPortTypeList(i, entry.graph->GetInputPort(i), true);

    if (ImGui::Button("Add Input")) {
        entry.graph->AddInput<Gin::Field::Sampler<Gin::Math::Scalar>>("New Input (" + std::to_string(entry.graph->GetInputsCount()) + ")");
    }
    ImGui::Separator();

    ImGui::TextUnformatted("Outputs : ");

    for (size_t i = 0; i < entry.graph->GetOutputsCount(); ++i)
        DrawGraphPortTypeList(i, entry.graph->GetOutputPort(i), false);

    if (ImGui::Button("Add Outputs")) {
        entry.graph->AddOutput<Gin::Field::Sampler<Gin::Math::Scalar>>("New Output (" + std::to_string(entry.graph->GetOutputsCount()) + ")");
    }
    ImGui::Separator();
}

void GraphEditorWindow::DrawGraphProgramToolTab() {
    static std::string infoText{};

    focusedAction = nullptr;
    GraphEntry& entry = graphs[currentGraphIdx];

    ImGui::TextUnformatted(infoText.c_str());

    if (ImGui::Button("Compile")) {
        std::chrono::time_point<std::chrono::system_clock> start{};

        start = std::chrono::system_clock::now();

        try {
            entry.graph->Compile();
        }
        catch (std::exception& e) {
            infoText = StringFormat("Graph Compilation Error : %s", e.what());
        }

        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
        infoText = StringFormat("Graph Compilation Took : %f s", elapsed.count() * 1000.0);
    }

    for (auto action = entry.graph->GetProgram().begin(); action != entry.graph->GetProgram().end(); ++action) {
        std::string text{};

        switch (action->type)
        {
            case Gin::Graph::GraphActionType::EXEC:
                text = StringFormat("Execute (%u)", action->nodeAId);
                break;
            case Gin::Graph::GraphActionType::COPY:
                text = StringFormat("Copy (%u)->(%u) to (%u)->(%u)\n",
                                    action->nodeBId, action->portBId, action->nodeAId, action->portAId);
                break;
            case Gin::Graph::GraphActionType::MOVE:
                text = StringFormat("Move (%u)->(%u) to (%u)->(%u)\n",
                                    action->nodeBId, action->portBId, action->nodeAId, action->portAId);
                break;
            case Gin::Graph::GraphActionType::FREE:
                text = StringFormat("Free (%u)\n", action->nodeAId);
                break;
            default:
                text = "Unknown Action";
                break;
        }

        ImGui::Selectable(text.c_str());
        if (ImGui::IsItemHovered()) {
            focusedAction = &*action;
        }
    }
}

const char* GetMeshBuilderTypeName(MeshBuilderType type) {
    switch (type) {
        case MeshBuilderType::MarchingCube:
            return "Marching Cube";
        case MeshBuilderType::SurfaceNet:
            return "Surface Net";
        default:
            return "Unknown";
    }
}

void GraphEditorWindow::DrawMeshBuilderToolTab() {
    static Gin::Graph::GraphContext ctx{};

    ImGui::TextUnformatted("Builder Type");
    if (ImGui::BeginCombo("##MeshBuilderType", GetMeshBuilderTypeName(meshBuilderType))) {

        //if (ImGui::Selectable(GetMeshBuilderTypeName(MeshBuilderType::MarchingCube)))
        //    meshBuilderType = MeshBuilderType::MarchingCube;
        if (ImGui::Selectable(GetMeshBuilderTypeName(MeshBuilderType::SurfaceNet)))
            meshBuilderType = MeshBuilderType::SurfaceNet;

        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::InputFloat("Scale", &ctx.scale);

    ImGui::PushItemWidth(75);

    ImGui::InputFloat("##Origin_X", &ctx.bounds.origin.x);
    ImGui::SameLine();
    ImGui::InputFloat("##Origin_Y", &ctx.bounds.origin.y);
    ImGui::SameLine();
    ImGui::InputFloat("##Origin_Z", &ctx.bounds.origin.z);
    ImGui::SameLine();
    ImGui::TextUnformatted("Origin");

    ImGui::InputFloat("##Extent_X", &ctx.bounds.extent.x);
    ImGui::SameLine();
    ImGui::InputFloat("##Extent_Y", &ctx.bounds.extent.y);
    ImGui::SameLine();
    ImGui::InputFloat("##Extent_Z", &ctx.bounds.extent.z);
    ImGui::SameLine();
    ImGui::TextUnformatted("Extent");

    ImGui::PopItemWidth();

    ImGui::Checkbox("Recalculate Normals", &recalculateNormal);
    ImGui::Separator();

    if (ImGui::Button("Build")) {
        BuildVolume(ctx);
    }
}

ImColor GetColorByType(Gin::Graph::PortType type) {
    if ((int)type & (int)Gin::Graph::PortType::Object) {
        return ImColor(70, 204, 65, 255);
    }
    if ((int)type & (int)Gin::Graph::PortType::Scalar) {
        return ImColor(204, 65, 65, 255);
    }
    if ((int)type & (int)Gin::Graph::PortType::Vector2) {
        return ImColor(204, 192, 65, 255);
    }
    if ((int)type & (int)Gin::Graph::PortType::Vector3) {
        return ImColor(204, 192, 65, 255);
    }
    if ((int)type & (int)Gin::Graph::PortType::Dynamic) {
        return ImColor(204, 204, 204, 255);
    }

    return ImColor(65, 128, 204, 255);
}

void DrawPinIcon(float size, const ImVec4& color, bool filled) {

    if (ImGui::IsRectVisible(ImVec2{ size, size })) {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        float offset = -(size * 0.25f * 0.25f);
        ImVec2 c = pos + ImVec2{ size / 2, size / 2 } + ImVec2{ offset * 0.5f, offset * 0.5f };

        if (filled)
            drawList->AddCircleFilled(c, size, ImColor(color));
        else
            drawList->AddCircle(c, size, ImColor(color));
    }
    ImGui::Dummy(ImVec2{size, size});
}

void GraphEditorWindow::DrawPin(Gin::Graph::Port &port, Gin::Graph::GraphId id, bool input, bool hasLink) {
    ax::NodeEditor::PinKind pinKind = input ? ax::NodeEditor::PinKind::Input : ax::NodeEditor::PinKind::Output;

    ax::NodeEditor::BeginPin(id, pinKind);
    ax::NodeEditor::PinPivotAlignment(ImVec2(input ? 0.0f: 1.0f, 0.5f));
    ax::NodeEditor::PinPivotSize(ImVec2(0, 0));

    bool filled = hasLink;

    ImGui::BeginHorizontal(id);

    if (input) {
        DrawPinIcon(5.0f, GetColorByType(port.GetType().type), filled);
        ImGui::Spring(0);
    } else {
        ImGui::Spring(1);
    }

    std::string name = port.GetName() + "(" + port.GetType().shortName + ")";
    if (((int)port.GetType().type & (int)Gin::Graph::PortType::Field) || port.GetType().type == Gin::Graph::PortType::Dynamic) {
        ImGui::TextUnformatted(name.c_str());
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(175, 175, 175, 255));
        ImGui::TextUnformatted(name.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::Spring(0);

    if (input && !hasLink) {
        if (port.GetType() == Gin::Graph::GetPortTypeInfo<int>()) {
            ImGui::PushItemWidth(50);
            ImGui::InputInt("##Input", (int*)port.GetProperty(), 0, 0);
            ImGui::PopItemWidth();
        }
        if (port.GetType() == Gin::Graph::GetPortTypeInfo<float>()) {
            ImGui::PushItemWidth(50);
            ImGui::InputFloat("##Input", (float*)port.GetProperty());
            ImGui::PopItemWidth();
        }
        if (port.GetType() == Gin::Graph::GetPortTypeInfo<double>()) {
            ImGui::PushItemWidth(50);
            ImGui::InputDouble("##Input", (double*)port.GetProperty());
            ImGui::PopItemWidth();
        }

        if (port.GetType() == Gin::Graph::GetPortTypeInfo<Gin::Field::Sampler<float>>()) {
            Gin::Field::Sampler<float>* f = (Gin::Field::Sampler<float>*)port.GetProperty();
            if (auto cf = f->GetField<Gin::Field::ConstantField<float>>()) {
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("##Input", &cf->Get());
                ImGui::PopItemWidth();
            }
        }
        if (port.GetType() == Gin::Graph::GetPortTypeInfo<Gin::Field::DynamicSampler>()) {
            Gin::Field::DynamicSampler* f = (Gin::Field::DynamicSampler*)port.GetProperty();
            if (auto cf = f->GetField<Gin::Field::ConstantField<float>>()) {
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("##Input", &cf->Get());
                ImGui::PopItemWidth();
            }
        }

    }

    if (!input) {
        DrawPinIcon(5.0f, GetColorByType(port.GetType().type), filled);
        ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
    }

    ImGui::EndHorizontal();
    ax::NodeEditor::EndPin();
}

void GraphEditorWindow::DrawGraphPortNode() {
    GraphEntry& entry = graphs[currentGraphIdx];

    ax::NodeEditor::BeginNode(GRAPH_INPUT_NODE_ID + 1);
    for (size_t i = 0; i < entry.graph->GetInputsCount(); ++i) {
        Gin::Graph::GraphPort& port = entry.graph->GetInputPort(i);
        DrawPin(port, GRAPH_ID_MAX - i - 1, false, port.GetLinks().size() > 0);
    }
    ax::NodeEditor::EndNode();

    ax::NodeEditor::BeginNode(GRAPH_OUTPUT_NODE_ID + 1);
    for (size_t i = 0; i < entry.graph->GetOutputsCount(); ++i) {
        Gin::Graph::GraphPort& port = entry.graph->GetOutputPort(i);
        DrawPin(port, GRAPH_ID_MAX - i - MAX_PORT - 1, true, port.GetLinks().size() > 0);
    }
    ax::NodeEditor::EndNode();
}

void GraphEditorWindow::DrawNode(Gin::Graph::GraphNodeOperator<Gin::Graph::Node>& node) {
    if (focusedAction &&
        (focusedAction->type == Gin::Graph::GraphActionType::EXEC ||
        focusedAction->type == Gin::Graph::GraphActionType::FREE) &&
        focusedAction->nodeAId == node.GetNodeId()) {
        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, ImVec4(1.0f, 0.5f, 0.05f, 1.0f));
        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_HovNodeBorder, ImVec4(1.0f, 0.5f, 0.05f, 1.0f));
        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_SelNodeBorder, ImVec4(1.0f, 0.5f, 0.05f, 1.0f));
    }

    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodeRounding, 4);

    NodeBuilder builder{};

    GraphEntry& entry = graphs[currentGraphIdx];

    builder.Begin(node.GetNodeId() + 1);

    builder.BeginHeader();
    ImGui::Spring(0);
    ImGui::TextUnformatted(node->GetName().c_str());
    ImGui::Spring(1);
    ImGui::Dummy(ImVec2{ 0, 28 });
    builder.EndHeader();

    builder.BeginContent();

    builder.BeginInput();
    for (size_t i = 0; i < node->GetInputPortCount(); ++i) {
        DrawPin(node->GetInputPort(i), i + (node.GetNodeId() + 1) * MAX_PORT,
                true, !entry.graph->GetAdj()[node.GetNodeId()][i].empty());
    }
    builder.EndInput();

    builder.BeginOutput();
    for (size_t i = 0; i < node->GetOutputPortCount(); ++i) {
        DrawPin(node->GetOutputPort(i), i + node->GetInputPortCount() + (node.GetNodeId() + 1) * MAX_PORT,
                false, !entry.graph->GetAdj()[node.GetNodeId()][node->GetInputPortCount() + i].empty());
    }
    builder.EndOutput();

    builder.EndContent();

    builder.End();

    ax::NodeEditor::PopStyleVar();

    if (focusedAction &&
        (focusedAction->type == Gin::Graph::GraphActionType::EXEC ||
        focusedAction->type == Gin::Graph::GraphActionType::FREE) &&
        focusedAction->nodeAId == node.GetNodeId()) {
        ax::NodeEditor::PopStyleColor(3);
    }
}

void GraphEditorWindow::DrawLinks() {
    GraphEntry& entry = graphs[currentGraphIdx];
    auto& adj = entry.graph->GetAdj();

    int i = 0;

    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_FlowDuration, 0.3f);

    for (Gin::Graph::GraphId nodeId = 0; nodeId < entry.graph->GetNodeCount(); ++nodeId) {
        Gin::Graph::GraphNodeOperator<Gin::Graph::Node> node = entry.graph->GetNode<Gin::Graph::Node>(nodeId);

        for (Gin::Graph::GraphId portId = 0; portId < node->GetInputPortCount(); ++portId) {

            int portAId = (int)(portId + (nodeId + 1) * MAX_PORT);
            for (std::pair<Gin::Graph::GraphId, Gin::Graph::GraphId>& link : adj[nodeId][portId]) {
                if (link.first == GRAPH_INPUT_NODE_ID) {
                    Gin::Graph::GraphId portBId = GRAPH_INPUT_NODE_ID - link.second - 1;
                    ax::NodeEditor::Link(i, portAId, portBId);
                }
                else {
                    Gin::Graph::GraphId portBId = link.second + (link.first + 1) * MAX_PORT;
                    ax::NodeEditor::Link(i, portAId, portBId);
                }

                if (focusedAction && focusedAction->type == Gin::Graph::GraphActionType::COPY
                    && focusedAction->nodeAId == nodeId && focusedAction->nodeBId == link.first
                    && focusedAction->portAId == portId && focusedAction->portBId == link.second) {
                    ax::NodeEditor::Flow(i, ax::NodeEditor::FlowDirection::Backward);
                }
                ++i;
            }
        }
    }

    for (Gin::Graph::GraphId portId = 0; portId < entry.graph->GetOutputsCount(); ++portId) {
        auto& port = entry.graph->GetOutputPort(portId);
        Gin::Graph::GraphId portAId = GRAPH_INPUT_NODE_ID - portId - MAX_PORT - 1;

        for (std::pair<Gin::Graph::GraphId, Gin::Graph::GraphId>& link : port.GetLinks()) {
            Gin::Graph::GraphId portBId = link.second + (link.first + 1) * MAX_PORT;
            ax::NodeEditor::Link(i, portAId, portBId);

            if (focusedAction && focusedAction->type == Gin::Graph::GraphActionType::COPY
                && focusedAction->nodeAId == GRAPH_OUTPUT_NODE_ID && focusedAction->nodeBId == link.first
                && focusedAction->portAId == portId && focusedAction->portBId == link.second) {
                ax::NodeEditor::Flow(i, ax::NodeEditor::FlowDirection::Backward);
            }
            ++i;
        }
    }

    ax::NodeEditor::PopStyleVar();
}

void GraphEditorWindow::HandleLinkCreation() {

    if (ax::NodeEditor::BeginCreate(ImColor(255, 255, 255), 2.0f)) {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId)) {
            if (inputPinId && outputPinId) {
                if (ax::NodeEditor::AcceptNewItem()) {
                    uint32_t start = inputPinId.Get();
                    uint32_t end = outputPinId.Get();
                    GraphEntry& entry = graphs[currentGraphIdx];

                    if (start > GRAPH_INPUT_NODE_ID - MAX_PORT * 2 && end > GRAPH_INPUT_NODE_ID - MAX_PORT * 2)
                        return;

                    if (start < GRAPH_INPUT_NODE_ID - MAX_PORT * 2 && end < GRAPH_INPUT_NODE_ID - MAX_PORT * 2) {
                        Gin::Graph::GraphId portAIdx = start % MAX_PORT;
                        Gin::Graph::GraphId portBIdx = end % MAX_PORT;
                        Gin::Graph::GraphId nodeAIdx = (start - portAIdx) / MAX_PORT - 1;
                        Gin::Graph::GraphId nodeBIdx = (end - portBIdx) / MAX_PORT - 1;

                        try {
                            entry.graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).Link(entry.graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx));
                        }
                        catch (std::exception& e) {
                            std::cout << "Graph Editor : " << e.what() << std::endl;
                        }
                    }else {
                        if (end > GRAPH_INPUT_NODE_ID - MAX_PORT * 2)
                            std::swap(start, end);

                        if (start > GRAPH_INPUT_NODE_ID - MAX_PORT) {
                            Gin::Graph::GraphId graphInputIdx = GRAPH_INPUT_NODE_ID - start - 1;
                            Gin::Graph::GraphId portBIdx = end % MAX_PORT;
                            Gin::Graph::GraphId nodeBIdx = (end - portBIdx) / MAX_PORT - 1;

                            try {
                                entry.graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx).LinkGraphInput(graphInputIdx);
                            }
                            catch (std::exception& e) {
                                std::cout << "Graph Editor : " << e.what() << std::endl;
                            }
                        }
                        else {
                            Gin::Graph::GraphId graphOutputIdx = GRAPH_INPUT_NODE_ID - start - MAX_PORT - 1;
                            Gin::Graph::GraphId portAIdx = end % MAX_PORT;
                            Gin::Graph::GraphId nodeAIdx = (end - portAIdx) / MAX_PORT - 1;

                            try {
                                entry.graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).LinkGraphOutput(graphOutputIdx);
                            }
                            catch (std::exception& e) {
                                std::cout << "Graph Editor : " << e.what() << std::endl;
                            }
                        }
                    }

                }
            }
        }

        ax::NodeEditor::PinId pinId;
        if (ax::NodeEditor::QueryNewNode(&pinId)) {
            if (ax::NodeEditor::AcceptNewItem()) {
                ax::NodeEditor::Suspend();
                ImGui::OpenPopup("Add Node");
                ax::NodeEditor::Resume();
            }
        }
    }
    ax::NodeEditor::EndCreate();

}

void GraphEditorWindow::HandleDeletion() {
    GraphEntry& entry = graphs[currentGraphIdx];

    static std::vector<Gin::Graph::GraphId> deleteNodeQueue{};

    if (ax::NodeEditor::BeginDelete()) {
        ax::NodeEditor::NodeId nodeId;
        while (ax::NodeEditor::QueryDeletedNode(&nodeId)) {
            if (nodeId.Get() >= GRAPH_OUTPUT_NODE_ID) {
                ax::NodeEditor::RejectDeletedItem();
            } else {
                deleteNodeQueue.push_back(nodeId.Get() - 1);
            }
        }

        /*ax::NodeEditor::LinkId linkId;
        ax::NodeEditor::PinId startId, endId;
        while (ax::NodeEditor::QueryDeletedLink(&linkId, &startId, &endId)) {
            Gin::Graph::GraphId portAId = startId.Get() % MAX_PORT;
            Gin::Graph::GraphId portBId = endId.Get() % MAX_PORT;
            //Calculate "Node & Port id"
            //Unlink
            if (ax::NodeEditor::AcceptDeletedItem(false)) {

            }
        }*/
    }
    ax::NodeEditor::EndDelete();

    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        ax::NodeEditor::LinkId deleteLinkQueue[32]{};
        int c = ax::NodeEditor::GetSelectedLinks(deleteLinkQueue, 32);
        for (int i = 0; i < c; ++i) {
            ax::NodeEditor::PinId startId, endId;
            ax::NodeEditor::GetLinkPins(deleteLinkQueue[i], &startId, &endId);
            Gin::Graph::GraphId portAId = startId.Get() % MAX_PORT;
            Gin::Graph::GraphId nodeAId = (startId.Get() - portAId) / MAX_PORT - 1;
            Gin::Graph::GraphId portBId = endId.Get() % MAX_PORT;
            Gin::Graph::GraphId nodeBId = (endId.Get() - portBId) / MAX_PORT - 1;

            if (startId.Get() >= GRAPH_INPUT_NODE_ID - MAX_PORT * 2) {
                Gin::Graph::GraphId outputPortId = MAX_PORT - (startId.Get() - (GRAPH_INPUT_NODE_ID - MAX_PORT * 2 - 1));
                entry.graph->GetNode<Gin::Graph::Node>(nodeBId).GetPort(portBId).UnlinkGraphOutput(outputPortId);
            } else if (endId.Get() >= GRAPH_INPUT_NODE_ID - MAX_PORT) {
                Gin::Graph::GraphId inputPortId = MAX_PORT - (endId.Get() - (GRAPH_INPUT_NODE_ID - MAX_PORT  - 1));
                entry.graph->GetNode<Gin::Graph::Node>(nodeAId).GetPort(portAId).UnlinkGraphInput(inputPortId);
            } else {
                entry.graph->GetNode<Gin::Graph::Node>(nodeAId).GetPort(portAId).Unlink(
                        entry.graph->GetNode<Gin::Graph::Node>(nodeBId).GetPort(portBId));
            }
        }
    }

    if (!deleteNodeQueue.empty()) {
        std::sort(deleteNodeQueue.rbegin(), deleteNodeQueue.rend());
        for (Gin::Graph::GraphId nodeId : deleteNodeQueue) {
            for (Gin::Graph::GraphId i = nodeId; i < entry.graph->GetNodeCount() - 1; ++i)
                ax::NodeEditor::SetNodePosition(i + 1, ax::NodeEditor::GetNodePosition(i + 2));
            entry.graph->RemoveNode(nodeId);
        }

        ax::NodeEditor::ClearSelection();
        deleteNodeQueue.clear();
    }
}

void GraphEditorWindow::BuildVolume(Gin::Graph::GraphContext &context) {
    GraphEntry& entry = graphs[currentGraphIdx];

    size_t volumeIdx = entry.graph->HasOutput("_Volume");

    if (volumeIdx == GRAPH_ID_MAX) {
        printf("Can't build volume : a Float _Volume output is required.");
        return;
    }

    Gin::Graph::GraphPort& volumePort = entry.graph->GetOutputPort(volumeIdx);

    if (volumePort.GetType().type != (Gin::Graph::PortType)((int)Gin::Graph::PortType::Scalar + (int)Gin::Graph::PortType::Field)) {
        printf("Can't build volume : a Float _Volume output is required.");
        return;
    }

    Gin::Profiler::Start("Graph Execution");

    try {
        entry.graph->Compile();
        entry.graph->Execute(context, threadPool);
    } catch (std::exception& e) {
        printf("Error while building volume : %s", e.what());
        return;
    }

    Gin::Profiler::Stop();

    Gin::Mesh::IndexedMesh indexedMesh{};

    Gin::Math::Vector3 size{ context.bounds.extent * 2.0 / context.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

    Gin::Field::Sampler<float>* volumeSampler = (Gin::Field::Sampler<float>*)volumePort.GetProperty();

    Gin::Mesh::MeshBuildData buildData{};
    buildData.mesh = &indexedMesh;
    buildData.scale = context.scale;
    buildData.bounds = context.bounds;
    buildData.volume = *volumeSampler;

    size_t colorIdx = entry.graph->HasOutput("_Color");
    if (colorIdx != GRAPH_ID_MAX) {
        Gin::Graph::GraphPort& colorPort = entry.graph->GetOutputPort(colorIdx);

        if (colorPort.GetType().type == (Gin::Graph::PortType)((int)Gin::Graph::PortType::Vector4 + (int)Gin::Graph::PortType::Field)) {
            buildData.color = *(Gin::Field::Sampler<Gin::Math::Vector4>*)colorPort.GetProperty();
        }
    }

    switch (meshBuilderType) {
        case MeshBuilderType::MarchingCube:
        {
            Gin::Mesh::MarchingCubeMeshBuilder builder{};
            builder.Build(buildData);
            indexedMesh.BuildIndex();
            break;
        }
        case MeshBuilderType::SurfaceNet: {
            Gin::Mesh::SurfaceNetMeshBuilder builder{};
            builder.Build(buildData);
            break;
        }
    }

    if (recalculateNormal)
        indexedMesh.RecalculateNormals();

    std::shared_ptr<ViewWindow> view = editor->GetEditorWindow<ViewWindow>();
    if (view) {
        view->GetCurrentScene().Clear();
        view->GetCurrentScene().AddMesh(indexedMesh, "Graph Build Mesh");
    }

    std::shared_ptr<ProfilerWindow> profiler = editor->GetEditorWindow<ProfilerWindow>();
    if (profiler) {
        profiler->UpdateSession();
    }
}