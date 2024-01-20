#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <chrono>
#include <memory>

namespace Gin::Profiler {

    struct Session {
        struct Event {
            enum class EventType {
                None,
                Allocation,
                Deallocation
            };

            uint64_t tickOffset{ 0 };
            EventType type{ EventType::None };
            uint64_t value{};
        };

        std::string name{};
        std::chrono::time_point<std::chrono::system_clock> startTime{};
        std::vector<Event> events{};
    };

    void Start(std::string_view sessionName);
    void Stop();
    void RecordAllocation(uint64_t size, void* addr);
    void RecordDeallocation(void* addr);
    std::shared_ptr<Session> GetLastSession();

}