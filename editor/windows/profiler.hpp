#pragma once

#include "../editor.hpp"
#include <gin/profiler/profiler.hpp>

class ProfilerWindow : public EditorWindow {
public:
    void Initialize() final;
    void Draw(bool* open) final;
    std::string_view GetName() final;

    void UpdateSession();

private:
    std::shared_ptr<Gin::Profiler::Session> session{ nullptr };
    std::vector<float> memoryUsage{};
    float maxMemoryAllocated{ 0 };
    double time{ 0.0 };
};