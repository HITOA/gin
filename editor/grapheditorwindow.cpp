#include "grapheditorwindow.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <misc/cpp/imgui_stdlib.cpp>

GraphEditorWindow::GraphEditorWindow()
{
	context = ImNodes::CreateContext();

	ImNodesIO& io = ImNodes::GetIO();
	io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
	io.MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyShift;


	ImNodesStyle& style = ImNodes::GetStyle();
	style.Colors[ImNodesCol_TitleBar] = IM_COL32(20, 20, 20, 255);
	style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(51, 51, 51, 255);
	style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(51, 51, 51, 255);
	style.Colors[ImNodesCol_Link] = IM_COL32(150, 150, 150, 255);
	style.Colors[ImNodesCol_LinkSelected] = IM_COL32(180, 180, 180, 255);
	style.Colors[ImNodesCol_LinkHovered] = IM_COL32(180, 180, 180, 255);

	graph = std::make_shared<Gin::Graph::Graph>();
}

GraphEditorWindow::~GraphEditorWindow()
{
	ImNodes::DestroyContext(context);
}

void GraphEditorWindow::Draw(bool* draw)
{
	if (ImGui::Begin("Graph Editor", draw)) {
		DrawToolWindow();

		ImGui::SameLine();

		ImNodes::BeginNodeEditor();

		const bool openPopup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
		if (!ImGui::IsAnyItemHovered() && openPopup)
			ImGui::OpenPopup("Add Node");

		DrawAddNodePopup();

		DrawGraphPortNode();

		for (size_t i = 0; i < graph->GetNodeCount(); ++i)
			DrawNode(graph->GetNode<Gin::Graph::Node>(i));

		DrawLink();

		ImNodes::MiniMap();
		ImNodes::EndNodeEditor();

		HandleLinkCreation();
		HandleLinkDeletion();
		HandleNodeDeletion();
	}
	ImGui::End();
}

std::string GraphEditorWindow::GetName()
{
	return std::string{ "Graph Editor" };
}

void GraphEditorWindow::SetGraph(std::shared_ptr<Gin::Graph::Graph> graph)
{
	this->graph = graph;
}

void GraphEditorWindow::DrawToolWindow()
{
	static const float minWidth = 400;
	const float width = ImGui::GetWindowWidth() * 0.15f > minWidth ? ImGui::GetWindowWidth() * 0.15f : minWidth;
	if (ImGui::BeginChild("Graph Editor Tool Window", ImVec2{ width, 0 })) {
		if (ImGui::Button("Compile")) {
			Compile();
		}

		ImGui::BeginTabBar("Graph Editor Tool Window Tab Bar");

		if (ImGui::BeginTabItem("Graph Port")) {
			DrawGraphPortTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Program")) {
			DrawProgramTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("History")) {
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::EndChild();
}

void GraphEditorWindow::DrawGraphPort(size_t idx, Gin::Graph::GraphPort& port, bool input)
{
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
			if (ImGui::Selectable("Int"))
				SetGraphPortType<int>(idx, input);
			if (ImGui::Selectable("Float"))
				SetGraphPortType<float>(idx, input);
			if (ImGui::Selectable("Double"))
				SetGraphPortType<double>(idx, input);

			ImGui::Separator();

			if (ImGui::Selectable("Vec2<Int>"))
				SetGraphPortType<Eigen::Vector2<int>>(idx, input);
			if (ImGui::Selectable("Vec2<Float>"))
				SetGraphPortType<Eigen::Vector2<float>>(idx, input);
			if (ImGui::Selectable("Vec2<Double>"))
				SetGraphPortType<Eigen::Vector2<double>>(idx, input);

			ImGui::Separator();

			if (ImGui::Selectable("Vec3<Int>"))
				SetGraphPortType<Eigen::Vector3<int>>(idx, input);
			if (ImGui::Selectable("Vec3<Float>"))
				SetGraphPortType<Eigen::Vector3<float>>(idx, input);
			if (ImGui::Selectable("Vec3<Double>"))
				SetGraphPortType<Eigen::Vector3<double>>(idx, input);

			if (!input) {
				ImGui::Separator();

				if (ImGui::Selectable("Spatial<Int>"))
					SetGraphPortType<Gin::Spatial::Spatial<int>>(idx, input);
				if (ImGui::Selectable("Spatial<Float>"))
					SetGraphPortType<Gin::Spatial::Spatial<float>>(idx, input);
				if (ImGui::Selectable("Spatial<Double>"))
					SetGraphPortType<Gin::Spatial::Spatial<double>>(idx, input);

				ImGui::Separator();

				if (ImGui::Selectable("Spatial<Vec2<Int>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector2<int>>>(idx, input);
				if (ImGui::Selectable("Spatial<Vec2<Float>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector2<float>>>(idx, input);
				if (ImGui::Selectable("Spatial<Vec2<Double>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector2<double>>>(idx, input);

				ImGui::Separator();

				if (ImGui::Selectable("Spatial<Vec3<Int>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector3<int>>>(idx, input);
				if (ImGui::Selectable("Spatial<Vec3<Float>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector3<float>>>(idx, input);
				if (ImGui::Selectable("Spatial<Vec3<Double>>"))
					SetGraphPortType<Gin::Spatial::Spatial<Eigen::Vector3<double>>>(idx, input);
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Delete")) {
			if (input)
				DeleteGraphInputByIdx(idx);
			else
				DeleteGraphOutputByIdx(idx);
		}

		ImGui::EndListBox();
	}
}

void GraphEditorWindow::DrawGraphPortTab()
{
	ImGui::TextUnformatted("Inputs : ");

	for (size_t i = 0; i < graph->GetInputsCount(); ++i)
		DrawGraphPort(i, graph->GetInputPort(i), true);

	if (ImGui::Button("Add Input")) {
		graph->AddInput<int>("New Int (" + std::to_string(graph->GetInputsCount()) + ")");
	}
	ImGui::Separator();
	
	ImGui::TextUnformatted("Outputs : ");

	for (size_t i = 0; i < graph->GetOutputsCount(); ++i)
		DrawGraphPort(i, graph->GetOutputPort(i), false);

	if (ImGui::Button("Add Outputs")) {
		graph->AddOutput<int>("New Int (" + std::to_string(graph->GetOutputsCount()) + ")");
	}
	ImGui::Separator();
}

void GraphEditorWindow::DrawProgramTab()
{
	bool focused = false;

	for (auto& action = graph->GetProgram().begin(); action != graph->GetProgram().end(); ++action) {
		std::string text{};

		switch (action->type)
		{
		case Gin::Graph::GraphActionType::EXEC:
			text = fmt::format("Execute ({})", action->nodeAIdx);
			break;
		case Gin::Graph::GraphActionType::COPY:
			text = fmt::format("Copy ({})->({}) to ({})->({})\n", action->nodeBIdx, action->portBIdx, action->nodeAIdx, action->portAIdx);
			break;														
		case Gin::Graph::GraphActionType::MOVE:						
			text = fmt::format("Move ({})->({}) to ({})->({})\n", action->nodeBIdx, action->portBIdx, action->nodeAIdx, action->portAIdx);
			break;
		default:
			printf("Unknown Action\n");
			break;
		}

		ImGui::Selectable(text.c_str());
		if (ImGui::IsItemHovered()) {
			focusedAction = &*action;
			focused = true;
		}
	}

	if (!focused)
		focusedAction = nullptr;
}

void GraphEditorWindow::DrawAddNodeMenu(std::string path, size_t idx, std::function<size_t(Gin::Graph::Graph&)> f, ImVec2 clickPos)
{
	size_t i = idx;
	do {
		++i;

		if (i >= path.size()) {
			if (ImGui::MenuItem(path.substr(idx, i - idx).c_str())) {
				ImNodes::SetNodeScreenSpacePos(f(*graph), clickPos);
			}
			return;
		}
	} while (path[i] != '/');

	if (ImGui::BeginMenu(path.substr(idx, i - idx).c_str())) {
		DrawAddNodeMenu(path, i + 1, f, clickPos);
		ImGui::EndMenu();
	}
}

void GraphEditorWindow::DrawAddNodePopup()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
	if (ImGui::BeginPopup("Add Node")) {
		const ImVec2 clickPos = ImGui::GetMousePosOnOpeningCurrentPopup();

		ImGui::TextUnformatted("Add Node");
		ImGui::Separator();
		
		for (auto& entry : Gin::Module::GetRegistry()) {
			DrawAddNodeMenu(entry.first, 0, entry.second, clickPos);
		}
		
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void GraphEditorWindow::SetPinColor(Gin::Graph::Port& port)
{
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Object) {
		ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(70, 204, 65, 255));
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(85, 220, 80, 255));
		return;
	}
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Number) {
		ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(204, 65, 65, 255));
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(220, 80, 80, 255));
		return;
	}
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Vector2) {
		ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(204, 192, 65, 255));
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(220, 192, 65, 255));
		return;
	}
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Vector3) {
		ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(204, 192, 65, 255));
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(220, 192, 65, 255));
		return;
	}

	ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(65, 128, 204, 255));
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(80, 140, 220, 255));
}

void GraphEditorWindow::DrawPin(Gin::Graph::Port& port, int idx, bool input)
{
	static std::string name{};

	SetPinColor(port);

	if (input) {
		ImNodes::BeginInputAttribute(idx);
	}
	else {
		ImNodes::BeginOutputAttribute(idx);
	}
	
	name = port.GetName() + "(" + port.GetType().shortName + ")";
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Spatial) {
		ImGui::TextUnformatted(name.c_str());
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(175, 175, 175, 255));
		ImGui::TextUnformatted(name.c_str());
		ImGui::PopStyleColor();
	}

	if (input) {
		ImGui::SameLine();

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

		if (port.GetType() == Gin::Graph::GetPortTypeInfo<Gin::Spatial::Spatial<int>>()) {
			ImGui::PushItemWidth(50);
			static int i{ 0 };
			Gin::Spatial::Spatial<int>& spatial = *(Gin::Spatial::Spatial<int>*)port.GetProperty();
			i = spatial[0];
			ImGui::InputInt("##Input", &i, 0, 0);
			std::fill(&spatial[0], &spatial[spatial.GetWidth() * spatial.GetHeight() * spatial.GetDepth() + 1], i);
			ImGui::PopItemWidth();
		}
		if (port.GetType() == Gin::Graph::GetPortTypeInfo<Gin::Spatial::Spatial<float>>()) {
			ImGui::PushItemWidth(50);
			static float f{ 0.0f };
			Gin::Spatial::Spatial<float>& spatial = *(Gin::Spatial::Spatial<float>*)port.GetProperty();
			f = spatial[0];
			ImGui::InputFloat("##Input", &f);
			std::fill(&spatial[0], &spatial[spatial.GetWidth() * spatial.GetHeight() * spatial.GetDepth() + 1], f);
			ImGui::PopItemWidth();
		}
		if (port.GetType() == Gin::Graph::GetPortTypeInfo<Gin::Spatial::Spatial<double>>()) {
			ImGui::PushItemWidth(50);
			static double d{ 0.0 };
			Gin::Spatial::Spatial<double>& spatial = *(Gin::Spatial::Spatial<double>*)port.GetProperty();
			d = spatial[0];
			ImGui::InputDouble("##Input", &d);
			std::fill(&spatial[0], &spatial[spatial.GetWidth() * spatial.GetHeight() * spatial.GetDepth() + 1], d);
			ImGui::PopItemWidth();
		}
	}
	

	if (input) {
		ImNodes::EndInputAttribute();
	}
	else {
		ImNodes::EndOutputAttribute();
	}

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void GraphEditorWindow::DrawPin(Gin::Graph::GraphPort& port, int idx, bool input)
{
	static std::string name{};

	SetPinColor(port);

	if (input) {
		ImNodes::BeginInputAttribute(idx);
	}
	else {
		ImNodes::BeginOutputAttribute(idx);
	}

	name = port.GetName() + "(" + port.GetType().shortName + ")";
	if ((int)port.GetType().type & (int)Gin::Graph::PortType::Spatial) {
		ImGui::TextUnformatted(name.c_str());
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(175, 175, 175, 255));
		ImGui::TextUnformatted(name.c_str());
		ImGui::PopStyleColor();
	}

	if (input) {
		ImNodes::EndInputAttribute();
	}
	else {
		ImNodes::EndOutputAttribute();
	}

	ImNodes::PopColorStyle();
}

void GraphEditorWindow::DrawGraphPortNode()
{
	//Inputs
	ImNodes::BeginNode(std::numeric_limits<int>::max());

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Graph Inputs");
	ImNodes::EndNodeTitleBar();

	for (size_t i = 0; i < graph->GetInputsCount(); ++i) {
		Gin::Graph::GraphPort& port = graph->GetInputPort(i);
		DrawPin(port, std::numeric_limits<int>::max() - i, false);
	}

	ImNodes::EndNode();

	//Output
	ImNodes::BeginNode(std::numeric_limits<int>::max() - 1);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Graph Outputs");
	ImNodes::EndNodeTitleBar();

	for (size_t i = 0; i < graph->GetOutputsCount(); ++i) {
		Gin::Graph::GraphPort& port = graph->GetOutputPort(i);
		DrawPin(port, std::numeric_limits<int>::max() - i - MAX_PORT, true);
	}

	ImNodes::EndNode();
}

void GraphEditorWindow::DrawNode(Gin::Graph::GraphNodeOperator<Gin::Graph::Node>& node)
{
	if (focusedAction)
		if (focusedAction->type == Gin::Graph::GraphActionType::EXEC && focusedAction->nodeAIdx == node.GetNodeIdx())
			ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(255, 200, 50, 255));

	ImNodes::BeginNode(node.GetNodeIdx());

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(node->GetName().c_str());
	ImNodes::EndNodeTitleBar();

	//Vertical

	/*for (size_t i = 0; i < node->GetInputPortCount(); ++i) {
		DrawPin(node->GetInputPort(i), i + node.GetNodeIdx() * MAX_PORT, true);
	}

	ImGui::Spacing();

	for (size_t i = 0; i < node->GetOutputPortCount(); ++i) {
		DrawPin(node->GetOutputPort(i), i + node->GetInputPortCount() + node.GetNodeIdx() * MAX_PORT, false);
	}*/

	//Horizontal

	ImGui::BeginGroup();
	for (size_t i = 0; i < node->GetInputPortCount(); ++i) {
		DrawPin(node->GetInputPort(i), i + node.GetNodeIdx() * MAX_PORT, true);
	}
	ImGui::EndGroup();

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(25, 0));
	ImGui::SameLine();

	ImGui::BeginGroup();
	for (size_t i = 0; i < node->GetOutputPortCount(); ++i) {
		DrawPin(node->GetOutputPort(i), i + node->GetInputPortCount() + node.GetNodeIdx() * MAX_PORT, false);
	}
	ImGui::EndGroup();

	ImNodes::EndNode();

	if (focusedAction)
		if (focusedAction->type == Gin::Graph::GraphActionType::EXEC && focusedAction->nodeAIdx == node.GetNodeIdx())
			ImNodes::PopColorStyle();
}

void GraphEditorWindow::DrawLink()
{
	for (size_t i = 0; i < links.size(); ++i) {
		if (focusedAction) {
			size_t start;
			size_t end;

			if (links[i].first >= std::numeric_limits<int>::max() - MAX_PORT && focusedAction->nodeBIdx == std::numeric_limits<size_t>::max())
				start = std::numeric_limits<int>::max() - focusedAction->portBIdx;
			else
				start = focusedAction->portBIdx + focusedAction->nodeBIdx * MAX_PORT;

			if (links[i].second >= std::numeric_limits<int>::max() - MAX_PORT * 2 && focusedAction->nodeAIdx == std::numeric_limits<size_t>::max() - 1)
				end = std::numeric_limits<int>::max() - focusedAction->portAIdx - MAX_PORT;
			else
				end = focusedAction->portAIdx + focusedAction->nodeAIdx * MAX_PORT;

			if (focusedAction->type == Gin::Graph::GraphActionType::COPY && start == links[i].first && end == links[i].second) {
				ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(255, 200, 50, 255));
				ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(255, 200, 50, 255));
				ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(255, 200, 50, 255));
				ImNodes::Link(i, links[i].first, links[i].second);
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				continue;
			}
		}

		ImNodes::Link(i, links[i].first, links[i].second);
	}
}

void GraphEditorWindow::HandleLinkCreation()
{
	int start, end;
	if (ImNodes::IsLinkCreated(&start, &end)) {
		if (start > std::numeric_limits<int>::max() - MAX_PORT * 2 && end > std::numeric_limits<int>::max() - MAX_PORT * 2)
			return;

		if (start < std::numeric_limits<int>::max() - MAX_PORT * 2 && end < std::numeric_limits<int>::max() - MAX_PORT * 2) {
			size_t portAIdx = start % MAX_PORT;
			size_t portBIdx = end % MAX_PORT;
			size_t nodeAIdx = (start - portAIdx) / MAX_PORT;
			size_t nodeBIdx = (end - portBIdx) / MAX_PORT;

			if (std::find(links.begin(), links.end(), std::make_pair((size_t)start, (size_t)end)) != std::end(links))
				return;

			try {
				graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).Link(graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx));
				links.emplace_back(start, end);
			}
			catch (std::exception& e) {
				Vin::Logger::Warn("Graph Editor : {}", e.what());
			}
		}
		else {
			if (start > std::numeric_limits<int>::max() - MAX_PORT) {
				size_t graphInputIdx = std::numeric_limits<int>::max() - start;
				size_t portBIdx = end % MAX_PORT;
				size_t nodeBIdx = (end - portBIdx) / MAX_PORT;

				try {
					graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx).LinkGraphInput(graphInputIdx);
					links.emplace_back(start, end);
				}
				catch (std::exception& e) {
					Vin::Logger::Warn("Graph Editor : {}", e.what());
				}
			}
			else {
				size_t graphOutputIdx = std::numeric_limits<int>::max() - end - MAX_PORT;
				size_t portAIdx = start % MAX_PORT;
				size_t nodeAIdx = (start - portAIdx) / MAX_PORT;

				try {
					graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).LinkGraphOutput(graphOutputIdx);
					links.emplace_back(start, end);
				}
				catch (std::exception& e) {
					Vin::Logger::Warn("Graph Editor : {}", e.what());
				}
			}
		}
	}
}

void GraphEditorWindow::HandleLinkDeletion()
{
	{
		int linkId;
		if (ImNodes::IsLinkDestroyed(&linkId)) {
			DeleteLinkById(linkId);
		}
	}
	{
		const int selectionCount = ImNodes::NumSelectedLinks();
		if (selectionCount > 0 && (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_X))) {
			static std::vector<int> linksId{};
			linksId.resize(selectionCount);
			ImNodes::GetSelectedLinks(linksId.data());
			std::sort(linksId.rbegin(), linksId.rend());
			for (auto linkId : linksId) {
				DeleteLinkById(linkId);
			}
		}
	}
}

void GraphEditorWindow::HandleNodeDeletion()
{
	const int selectionCount = ImNodes::NumSelectedNodes();
	if (selectionCount > 0 && (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_X))) {
		static std::vector<int> nodesId{};
		nodesId.resize(selectionCount);
		ImNodes::GetSelectedNodes(nodesId.data());
		std::sort(nodesId.rbegin(), nodesId.rend());
		for (auto nodeId : nodesId) {
			DeleteNodeById(nodeId);
		}
	}
}

void GraphEditorWindow::DeleteLinkById(int id)
{
	std::pair<size_t, size_t> link = links[id];

	if (link.first < std::numeric_limits<int>::max() - MAX_PORT * 2 && link.second < std::numeric_limits<int>::max() - MAX_PORT * 2) {

		size_t portAIdx = link.first % MAX_PORT;
		size_t portBIdx = link.second % MAX_PORT;
		size_t nodeAIdx = (link.first - portAIdx) / MAX_PORT;
		size_t nodeBIdx = (link.second - portBIdx) / MAX_PORT;

		try {
			graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).Unlink(graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx));
			links.erase(links.begin() + id);
		}
		catch (std::exception& e) {
			Vin::Logger::Warn("Graph Editor : {}", e.what());
		}
	}
	else {
		if (link.first > std::numeric_limits<int>::max() - MAX_PORT) {
			size_t graphInputIdx = std::numeric_limits<int>::max() - link.first;
			size_t portBIdx = link.second % MAX_PORT;
			size_t nodeBIdx = (link.second - portBIdx) / MAX_PORT;

			try {
				graph->GetNode<Gin::Graph::Node>(nodeBIdx).GetPort(portBIdx).UnlinkGraphInput(graphInputIdx);
				links.erase(links.begin() + id);
			}
			catch (std::exception& e) {
				Vin::Logger::Warn("Graph Editor : {}", e.what());
			}
		}
		else {
			size_t graphOutputIdx = std::numeric_limits<int>::max() - link.second - MAX_PORT;
			size_t portAIdx = link.first % MAX_PORT;
			size_t nodeAIdx = (link.first - portAIdx) / MAX_PORT;

			try {
				graph->GetNode<Gin::Graph::Node>(nodeAIdx).GetPort(portAIdx).UnlinkGraphOutput(graphOutputIdx);
				links.erase(links.begin() + id);
			}
			catch (std::exception& e) {
				Vin::Logger::Warn("Graph Editor : {}", e.what());
			}
		}
	}
}

void GraphEditorWindow::DeleteNodeById(int id)
{
	if (id >= std::numeric_limits<int>::max() - 1)
		return;

	for (size_t i = (size_t)id; i < graph->GetNodeCount() - 1; ++i) {
		ImNodes::SetNodeScreenSpacePos(i, ImNodes::GetNodeScreenSpacePos(i + 1));
	}

	graph->RemoveNode(id);

	for (size_t i = 0; i < links.size();) {
		size_t portAIdx = links[i].first % MAX_PORT;
		size_t portBIdx = links[i].second % MAX_PORT;
		size_t nodeAIdx = (links[i].first - portAIdx) / MAX_PORT;
		size_t nodeBIdx = (links[i].second - portBIdx) / MAX_PORT;

		if (nodeAIdx == id || nodeBIdx == id) {
			links.erase(links.begin() + i);
			continue;
		}

		if (nodeAIdx > id && links[i].first < std::numeric_limits<int>::max() - MAX_PORT * 2)
			links[i].first -= MAX_PORT;
		if (nodeBIdx > id && links[i].second < std::numeric_limits<int>::max() - MAX_PORT * 2)
			links[i].second -= MAX_PORT;

		++i;
	}
}

void GraphEditorWindow::DeleteGraphInputByIdx(size_t idx)
{
	graph->RemoveInput(idx);

	int id = std::numeric_limits<int>::max() - idx;

	for (size_t i = 0; i < links.size();) {
		if (links[i].first == id) {
			links.erase(links.begin() + i);
			continue;
		}

		if (links[i].first > std::numeric_limits<int>::max() - MAX_PORT && links[i].first < id)
			links[i].first += 1;

		++i;
	}
}

void GraphEditorWindow::DeleteGraphOutputByIdx(size_t idx)
{
	graph->RemoveOutput(idx);

	int id = std::numeric_limits<int>::max() - idx - MAX_PORT;

	for (size_t i = 0; i < links.size();) {
		if (links[i].second == id) {
			links.erase(links.begin() + i);
			continue;
		}

		if (links[i].second > std::numeric_limits<int>::max() - MAX_PORT * 2 && links[i].second < id)
			links[i].second += 1;

		++i;
	}
}

void GraphEditorWindow::Compile()
{
	std::chrono::time_point<std::chrono::system_clock> start{};

	start = std::chrono::system_clock::now();

	try {
		graph->Compile();
	}
	catch (std::exception& e) {
		Vin::Logger::Err("Graph Compilation Error : {}", e.what());
	}

	std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
	Vin::Logger::Log("Graph Compilation Took : {}ms", elapsed.count() * 1000.0);
}

void GraphEditorWindow::Execute(Gin::Graph::GraphContext ctx)
{
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