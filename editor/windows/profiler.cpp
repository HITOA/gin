#include "profiler.hpp"
#include "../stringfmt.hpp"
#include "grapheditor.hpp"

void ProfilerWindow::Initialize() {

}

void ProfilerWindow::Draw(bool *open) {
    static bool windowFilter[] = { true, true };
    if (ImGui::Begin("Profiler", open, ImGuiWindowFlags_MenuBar)) {
        ImVec2 availableRegion = ImGui::GetContentRegionAvail();

        if (ImGui::BeginMenuBar()) {
            ImGui::BeginHorizontal("ProfilerBarHorizontal", ImVec2{ availableRegion.x, 0});
            if (ImGui::BeginMenu(u8"\uF0B0 Filter")) {
                ImGui::Checkbox("Event Graph", &windowFilter[0]);
                ImGui::Checkbox("Memory Usage Graph", &windowFilter[1]);

                ImGui::EndMenu();
            }

            ImGui::Spring(1.0f);
            ImGui::Text("Total Duration : %fs", time);
            ImGui::EndHorizontal();

            ImGui::EndMenuBar();
        }

        if (windowFilter[0] && windowFilter[1]) {
            availableRegion.y *= 0.5f;
            availableRegion.y -= GImGui->Style.FramePadding.y;
            DrawEventGraph(availableRegion);
            DrawMemoryUsageGraph(availableRegion);
        } else {
            if (windowFilter[0])
                DrawEventGraph(availableRegion);
            if (windowFilter[1])
                DrawMemoryUsageGraph(availableRegion);
        }
    }
    ImGui::End();
}

std::string_view ProfilerWindow::GetName() {
    return "Profiler";
}

void ProfilerWindow::UpdateSession() {
    session = Gin::Profiler::GetLastSession();

    if (session->events.size() < 1)
        return;

    threads.clear();
    events.clear();
    maxMemoryAllocated = 0;

    std::chrono::duration<float> sd = session->endTime - session->startTime;
    time = sd.count();
    tpp = session->events[session->events.size() - 1].tickOffset / MAX_TICK_IN_GRAPH;

    std::vector<std::vector<ExtendedNodeEvent>> threadEvents{};

    GraphEntry* graphEntry{};
    std::shared_ptr<GraphEditorWindow> graphEditor = editor->GetEditorWindow<GraphEditorWindow>();
    if (graphEditor)
        graphEntry = &graphEditor->GetCurrentGraph();

    uint64_t currentMemoryAllocated = 0;

    for (auto& event : session->events) {
        int threadIdx = -1;
        for (int i = 0; i < threads.size(); ++i)
        {
            if (threads[i] == event.tid){
                threadIdx = i;
                break;
            }
        }
        if (threadIdx == -1) {
            threadIdx = threads.size();
            threads.push_back(event.tid);
            threadEvents.emplace_back();
        }

        if (event.type == Gin::Profiler::Session::Event::EventType::EnterNode) {
            ExtendedNodeEvent ene{};
            ene.threadIdx = threadIdx;
            ene.tickStart = event.tickOffset;
            if (event.owner == std::numeric_limits<uint32_t>::max())
                ene.nodeName = "SubEvent";
            else
                ene.nodeName = graphEntry->graph->GetNode<Gin::Graph::Node>(event.owner)->GetName();
            ene.nodeId = event.owner;
            threadEvents[threadIdx].push_back(ene);
        }

        if (event.type == Gin::Profiler::Session::Event::EventType::LeaveNode) {
            ExtendedNodeEvent& ene = threadEvents[threadIdx].back();
            ene.tickDuration = event.tickOffset - ene.tickStart;
            ene.durationInSeconds = time * ((double)ene.tickDuration / (double)session->events[session->events.size() - 1].tickOffset);
            events.push_back(ene);
            threadEvents[threadIdx].pop_back();
        }

        if (event.type == Gin::Profiler::Session::Event::EventType::Allocation) {
            currentMemoryAllocated += event.value;
            if (currentMemoryAllocated > maxMemoryAllocated)
                maxMemoryAllocated = currentMemoryAllocated;
        }

        if (event.type == Gin::Profiler::Session::Event::EventType::Deallocation) {
            currentMemoryAllocated -= event.value;
        }
    }
}

void ProfilerWindow::DrawEventGraph(ImVec2 size) {
    static const float eventGraphBBXOffset = 200.0;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImGuiStyle& style = g.Style;

    const ImVec2 frameSize = ImGui::CalcItemSize(size, ImGui::CalcItemWidth(), style.FramePadding.y * 2.0f);

    ImRect frameBB(window->DC.CursorPos, window->DC.CursorPos);
    frameBB.Max.x += frameSize.x;
    frameBB.Max.y += frameSize.y;

    ImGui::ItemSize(frameBB, style.FramePadding.y);
    if (!ImGui::ItemAdd(frameBB, 0, &frameBB))
        return;

    ImRect innerBB(frameBB.Min, frameBB.Max);
    innerBB.Min.x += style.FramePadding.x;
    innerBB.Min.y += style.FramePadding.y;
    innerBB.Max.x -= style.FramePadding.x;
    innerBB.Max.y -= style.FramePadding.y;

    ImRect eventGraphFrameBB = frameBB;
    eventGraphFrameBB.Min.x += eventGraphBBXOffset;
    ImRect eventGraphBB = innerBB;
    eventGraphBB.Min.x += eventGraphBBXOffset;

    float eventGraphW = eventGraphBB.Max.x - eventGraphBB.Min.x;
    float eventGraphH = eventGraphBB.Max.y - eventGraphBB.Min.y;
    float rectH = eventGraphH / (float)(threads.size());
    float tickW = session ? eventGraphW / ((float)session->events[session->events.size() - 1].tickOffset / tpp) : 0;

    ImGui::RenderFrame(eventGraphFrameBB.Min, eventGraphFrameBB.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    for (int i = 0; i < threads.size(); ++i) {
        ImVec2 p1 = eventGraphBB.Min;
        ImVec2 p2 = eventGraphBB.Min;
        p1.y += rectH * i;
        p1.x -= style.FramePadding.x;
        p2.y += rectH * i;
        p2.x = eventGraphBB.Max.x + style.FramePadding.x;
        window->DrawList->AddLine(p1, p2, ImGui::GetColorU32(ImGuiCol_Separator));

        p1.x += style.FramePadding.x - eventGraphBBXOffset;
        p1.y += rectH / 2.0 - 6.0f;

        std::string ft = StringFormat("Thread 0x%lx", threads[i]);
        window->DrawList->AddText(p1, ImGui::GetColorU32(ImGuiCol_Text), ft.c_str());
    }

    for (auto event : events) {
        ImRect eventBB = eventGraphBB;
        eventBB.Min.x += tickW * (event.tickStart / tpp);
        eventBB.Min.y += rectH * event.threadIdx;
        eventBB.Max.x = eventBB.Min.x + tickW * (event.tickDuration / tpp < 1.0 ? 1.0 : (event.tickDuration / tpp));
        eventBB.Max.y = eventBB.Min.y + rectH;

        if (eventBB.Contains(g.IO.MousePos)) {
            ImGui::SetTooltip("Node : %s (%lu)\nDuration : %fms", event.nodeName.c_str(), event.nodeId, event.durationInSeconds * 1000.0);
            window->DrawList->AddRectFilled(eventBB.Min, eventBB.Max, ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered));
        } else {
            window->DrawList->AddRectFilled(eventBB.Min, eventBB.Max, ImGui::GetColorU32(ImGuiCol_PlotHistogram));
        }

        std::string ft = StringFormat("%s (%lu)", event.nodeName.c_str(), event.nodeId);
        ImVec2 textSize = ImGui::CalcTextSize(ft.c_str());
        ImVec2 textPos = eventBB.Min;
        textPos.y += (eventBB.Max.y - eventBB.Min.y) / 2.0f - textSize.y / 2.0f;
        textPos.x += style.FramePadding.x;
        if (textSize.x < eventBB.Max.x - eventBB.Min.x)
            window->DrawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), ft.c_str());
    }

}

void ProfilerWindow::DrawMemoryUsageGraph(ImVec2 size) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImGuiStyle& style = g.Style;

    const ImVec2 frameSize = ImGui::CalcItemSize(size, ImGui::CalcItemWidth(), style.FramePadding.y * 2.0f);

    ImRect frameBB(window->DC.CursorPos, window->DC.CursorPos);
    frameBB.Max.x += frameSize.x;
    frameBB.Max.y += frameSize.y;

    ImGui::ItemSize(frameBB, style.FramePadding.y);
    if (!ImGui::ItemAdd(frameBB, 0, &frameBB))
        return;

    ImRect innerBB(frameBB.Min, frameBB.Max);
    innerBB.Min.x += style.FramePadding.x;
    innerBB.Min.y += style.FramePadding.y;
    innerBB.Max.x -= style.FramePadding.x;
    innerBB.Max.y -= style.FramePadding.y;

    float memoryGraphW = innerBB.Max.x - innerBB.Min.x;
    float memoryGraphH = innerBB.Max.y - innerBB.Min.y;
    float heightFactor = memoryGraphH / (float)maxMemoryAllocated;
    float tickW = session ? memoryGraphW / ((float)session->events[session->events.size() - 1].tickOffset / tpp) : 0;

    ImGui::RenderFrame(frameBB.Min, frameBB.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (!session)
        return;

    uint64_t lastMemoryAllocated = 0;
    uint64_t lastEventTick = 0;

    for (auto event : session->events) {
        if (event.type == Gin::Profiler::Session::Event::EventType::Allocation) {
            uint64_t currentMemoryAllocated = lastMemoryAllocated + event.value;
            ImVec2 p1 = innerBB.Min;
            p1.x += tickW * lastEventTick;
            p1.y = innerBB.Max.y;
            ImVec2 p2 = p1;
            p2.x = innerBB.Min.x + tickW * (event.tickOffset / tpp);
            ImVec2 p3 = p2;
            p2.y -= currentMemoryAllocated * heightFactor;
            ImVec2 p4 = p1;
            p1.y -= lastMemoryAllocated * heightFactor;
            window->DrawList->AddQuadFilled(p1, p2, p3, p4, ImGui::GetColorU32(ImGuiCol_PlotHistogram));
            lastMemoryAllocated = currentMemoryAllocated;
            lastEventTick = event.tickOffset / tpp;
        } else if (event.type == Gin::Profiler::Session::Event::EventType::Deallocation) {
            uint64_t currentMemoryAllocated = lastMemoryAllocated - event.value;
            ImVec2 p1 = innerBB.Min;
            p1.x += tickW * lastEventTick;
            p1.y = innerBB.Max.y;
            ImVec2 p2 = p1;
            p2.x = innerBB.Min.x + tickW * (event.tickOffset / tpp);
            ImVec2 p3 = p2;
            p2.y -= currentMemoryAllocated * heightFactor;
            ImVec2 p4 = p1;
            p1.y -= lastMemoryAllocated * heightFactor;
            window->DrawList->AddQuadFilled(p1, p2, p3, p4, ImGui::GetColorU32(ImGuiCol_PlotHistogram));
            lastMemoryAllocated = currentMemoryAllocated;
            lastEventTick = event.tickOffset / tpp;
        }
    }

    if (innerBB.Contains(g.IO.MousePos)) {
        uint64_t cm = std::abs(g.IO.MousePos.y - innerBB.Max.y) / heightFactor;
        double ct = (g.IO.MousePos.x - innerBB.Min.x) / memoryGraphW * time;
        ImGui::SetTooltip("Memory Allocated : %f MB\nTime : %fs", ((double)cm / 8000000.0), ct);
    }
}