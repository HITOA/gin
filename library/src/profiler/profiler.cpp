#include <gin/profiler/profiler.hpp>
#include <unordered_map>
#include <mutex>

std::shared_ptr<Gin::Profiler::Session> session{};
bool running{ false };
uint64_t lastOffset{ 0 };
std::unordered_map<void*, uint64_t> allocation{};
std::mutex mutex{};
thread_local std::vector<uint32_t> nodes{};

void Gin::Profiler::Start(std::string_view sessionName) {
    session = std::make_shared<Gin::Profiler::Session>();
    session->name = sessionName;
    session->startTime = std::chrono::system_clock::now();
    running = true;
}

void Gin::Profiler::Stop() {
    if (running) {
        session->endTime = std::chrono::system_clock::now();
        allocation.clear();
        nodes.clear();
        running = false;
    }
}

void Gin::Profiler::RecordAllocation(uint64_t size, void* ptr) {
    if (!running)
        return;

    std::unique_lock<std::mutex> lock{ mutex };

    allocation[ptr] = size;

    Session::Event event{};
    event.type = Session::Event::EventType::Allocation;
    lastOffset = event.tickOffset;
    event.value = size;
    event.tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
    event.tickOffset = (std::chrono::system_clock::now() - session->startTime).count() - lastOffset;
    session->events.push_back(event);
}

void Gin::Profiler::RecordDeallocation(void* ptr) {
    if (!running)
        return;

    std::unique_lock<std::mutex> lock{ mutex };

    if (allocation.count(ptr) <= 0)
        return;

    Session::Event event{};
    event.type = Session::Event::EventType::Deallocation;
    lastOffset = event.tickOffset;
    event.value = allocation[ptr];
    event.tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
    if (nodes.size() > 0)
        event.owner = nodes[nodes.size() - 1];
    allocation.erase(ptr);
    event.tickOffset = (std::chrono::system_clock::now() - session->startTime).count() - lastOffset;
    session->events.push_back(event);
}

void Gin::Profiler::RecordEnterNode(uint32_t node) {
    if (!running)
        return;

    std::unique_lock<std::mutex> lock{ mutex };

    nodes.push_back(node);

    Session::Event event{};
    event.type = Session::Event::EventType::EnterNode;
    lastOffset = event.tickOffset;
    event.tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
    event.owner = node;
    event.tickOffset = (std::chrono::system_clock::now() - session->startTime).count() - lastOffset;
    session->events.push_back(event);
}

void Gin::Profiler::RecordLeaveNode() {
    if (!running)
        return;

    std::unique_lock<std::mutex> lock{ mutex };

    Session::Event event{};
    event.type = Session::Event::EventType::LeaveNode;
    lastOffset = event.tickOffset;
    event.tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
    event.owner = nodes[nodes.size() - 1];
    event.tickOffset = (std::chrono::system_clock::now() - session->startTime).count() - lastOffset;
    session->events.push_back(event);

    nodes.pop_back();
}

std::shared_ptr<Gin::Profiler::Session> Gin::Profiler::GetLastSession() {
    if (running)
        return nullptr;
    return session;
}