#include "profiler.hpp"

void ProfilerWindow::Initialize() {

}

void ProfilerWindow::Draw(bool *open) {
    if (ImGui::Begin("Profiler", open)) {
        ImGui::Text("Time : %f s", time);
        ImGui::PlotHistogram("Memory Usage", memoryUsage.data(), memoryUsage.size(), 0, "Memory Usage (MB)", 0, maxMemoryAllocated, ImGui::GetContentRegionAvail());
    }
    ImGui::End();
}

std::string_view ProfilerWindow::GetName() {
    return "Profiler";
}

void ProfilerWindow::UpdateSession() {
    session = Gin::Profiler::GetLastSession();

    float memoryF{ 1e-6 };
    uint64_t currTickPosition{ 0 };
    uint64_t tickPerPoint{ session->events[session->events.size() - 1].tickOffset / 100 };
    float currMemoryAllocated{ 0 };
    maxMemoryAllocated = 0;

    memoryUsage.clear();

    for (auto& event : session->events) {
        while (currTickPosition < event.tickOffset) {
            memoryUsage.emplace_back(currMemoryAllocated);
            currTickPosition += tickPerPoint;
        }
        switch (event.type) {
            case Gin::Profiler::Session::Event::EventType::Allocation:
                currMemoryAllocated += event.value * memoryF;
                break;
            case Gin::Profiler::Session::Event::EventType::Deallocation:
                currMemoryAllocated -= event.value * memoryF;
                break;
        }

        if (currMemoryAllocated > maxMemoryAllocated)
            maxMemoryAllocated = currMemoryAllocated;
    }

    std::chrono::duration<float> sd = session->endTime - session->startTime;
    time = sd.count();

    memoryUsage.emplace_back(currMemoryAllocated);
}