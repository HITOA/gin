#pragma once

#include "../editor.hpp"
#include <gin/profiler/profiler.hpp>

#define MAX_TICK_IN_GRAPH 100

class ProfilerWindow : public EditorWindow {
public:
    void Initialize() final;
    void Draw(bool* open) final;
    std::string_view GetName() final;

    void UpdateSession();
    void DrawEventGraph();

private:
    struct ExtendedNodeEvent {
        uint32_t threadIdx{};
        uint64_t tickStart{ 0 };
        uint64_t tickDuration{ 0 };
        double durationInSeconds{};
        std::string nodeName{};
        uint64_t nodeId{};
    };

    std::shared_ptr<Gin::Profiler::Session> session{ nullptr };
    std::vector<uint64_t> threads{};
    uint64_t tpp{ 0 };
    std::vector<ExtendedNodeEvent> events{};
    double time{ 0.0 };
};