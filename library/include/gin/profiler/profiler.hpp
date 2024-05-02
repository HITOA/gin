#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <chrono>
#include <memory>
#include <thread>

namespace Gin::Profiler {

    struct Session {
        struct Event {
            enum class EventType {
                None,
                EnterNode,
                LeaveNode,
                Allocation,
                Deallocation
            };

            uint64_t tickOffset{ 0 };
            EventType type{ EventType::None };
            uint64_t value{};
            uint32_t owner{};
            uint64_t tid{};
        };

        std::string name{};
        std::chrono::time_point<std::chrono::system_clock> startTime{};
        std::chrono::time_point<std::chrono::system_clock> endTime{};
        std::vector<Event> events{};
    };

    void Start(std::string_view sessionName);
    void Stop();
    void RecordAllocation(uint64_t size, void* addr);
    void RecordDeallocation(void* addr);
    void RecordEnterNode(uint32_t node);
    void RecordLeaveNode();
    std::shared_ptr<Session> GetLastSession();

}