#include "profiler.hpp"
#include "../stringfmt.hpp"
#include "grapheditor.hpp"

/*int ProfilerCustomPlot(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2& size_arg)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return -1;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImVec2 frame_size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);

    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos);
    frame_bb.Max.x += frame_size.x;
    frame_bb.Max.y += frame_size.y;

    ImRect inner_bb(frame_bb.Min, frame_bb.Max);
    inner_bb.Min.x -= style.FramePadding.x;
    inner_bb.Min.y -= style.FramePadding.y;
    inner_bb.Max.x += style.FramePadding.x;
    inner_bb.Max.y += style.FramePadding.y;

    ImRect total_bb(frame_bb.Min, frame_bb.Max);
    total_bb.Max.x += ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0).x;
    total_bb.Max.y += ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0).y;

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, 0, &frame_bb))
        return -1;
    const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data, i);
            if (v != v) // Ignore NaN values
                continue;
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    const int values_count_min = (plot_type == ImGuiPlotType_Lines) ? 2 : 1;
    int idx_hovered = -1;
    if (values_count >= values_count_min)
    {
        int res_w = ImMin((int)frame_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        // Tooltip on hover
        if (hovered && inner_bb.Contains(g.IO.MousePos))
        {
            const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
            const int v_idx = (int)(t * item_count);
            IM_ASSERT(v_idx >= 0 && v_idx < values_count);

            const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
            const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
            if (plot_type == ImGuiPlotType_Lines)
                ImGui::SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx + 1, v1);
            else if (plot_type == ImGuiPlotType_Histogram)
                ImGui::SetTooltip("%8.4g MB", v0);
            idx_hovered = v_idx;
        }

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (1 + scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = ImGui::GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
        const ImU32 col_hovered = ImGui::GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, histogram_zero_line_t));
            if (plot_type == ImGuiPlotType_Lines)
            {
                window->DrawList->AddLine(pos0, pos1, idx_hovered == v1_idx ? col_hovered : col_base);
            }
            else if (plot_type == ImGuiPlotType_Histogram)
            {
                if (pos1.x >= pos0.x + 2.0f)
                    pos1.x -= 1.0f;
                window->DrawList->AddRectFilled(pos0, pos1, idx_hovered == v1_idx ? col_hovered : col_base);
            }

            t0 = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        ImGui::RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f, 0.0f));

    if (label_size.x > 0.0f)
        ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);

    // Return hovered index or -1 if none are hovered.
    // This is currently not exposed in the public API because we need a larger redesign of the whole thing, but in the short-term we are making it available in PlotEx().
    return idx_hovered;
}*/

void ProfilerWindow::Initialize() {

}

void ProfilerWindow::Draw(bool *open) {
    if (ImGui::Begin("Profiler", open)) {
        //ImGui::PlotHistogram("Total Memory Usage", memoryUsage.data(), memoryUsage.size(), 0, "Total Memory Usage (MB)", 0, maxMemoryAllocated * 1.1, ImGui::GetContentRegionAvail());
        DrawEventGraph();
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

    std::chrono::duration<float> sd = session->endTime - session->startTime;
    time = sd.count();
    tpp = session->events[session->events.size() - 1].tickOffset / MAX_TICK_IN_GRAPH;

    std::vector<std::vector<ExtendedNodeEvent>> threadEvents{};

    GraphEntry* graphEntry{};
    std::shared_ptr<GraphEditorWindow> graphEditor = editor->GetEditorWindow<GraphEditorWindow>();
    if (graphEditor)
        graphEntry = &graphEditor->GetCurrentGraph();

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
    }
}

void ProfilerWindow::DrawEventGraph() {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImGuiStyle& style = g.Style;
    //const ImGuiID id = window->GetID(label);

    ImVec2 size = ImGui::GetContentRegionAvail();

    const ImVec2 frameSize = ImGui::CalcItemSize(size, ImGui::CalcItemWidth(), style.FramePadding.y * 2.0f);

    ImRect frameBB(window->DC.CursorPos, window->DC.CursorPos);
    frameBB.Max.x += frameSize.x;
    frameBB.Max.y += frameSize.y;

    ImRect innerBB(frameBB.Min, frameBB.Max);
    innerBB.Min.x += style.FramePadding.x;
    innerBB.Min.y += style.FramePadding.y;
    innerBB.Max.x -= style.FramePadding.x;
    innerBB.Max.y -= style.FramePadding.y;

    ImRect eventGraphBB = innerBB;
    eventGraphBB.Min.x += 200.0;

    float eventGraphW = eventGraphBB.Max.x - eventGraphBB.Min.x;
    float eventGraphH = eventGraphBB.Max.y - eventGraphBB.Min.y;
    float rectH = eventGraphH / (float)(threads.size());
    float tickW = session ? eventGraphW / ((float)session->events[session->events.size() - 1].tickOffset / tpp) : 0;

    ImGui::RenderFrame(frameBB.Min, frameBB.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    {
        ImVec2 p1 = eventGraphBB.Min;
        ImVec2 p2 = eventGraphBB.Min;
        p1.x -= 12.5;
        p2.x -= 12.5;
        p2.y = eventGraphBB.Max.y;
        window->DrawList->AddLine(p1, p2, ImGui::GetColorU32(ImGuiCol_Separator));
    }

    for (int i = 0; i < threads.size(); ++i) {
        ImVec2 p1 = innerBB.Min;
        ImVec2 p2 = innerBB.Min;
        p1.y += rectH * i;
        p1.x -= style.FramePadding.x;
        p2.y += rectH * i;
        p2.x = innerBB.Max.x + style.FramePadding.x;
        window->DrawList->AddLine(p1, p2, ImGui::GetColorU32(ImGuiCol_Separator));

        p1.x += style.FramePadding.x;
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
            ImGui::SetTooltip("Node : %s (%lu)\nDuration : %fs", event.nodeName.c_str(), event.nodeId, event.durationInSeconds);
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