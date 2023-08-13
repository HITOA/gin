#include <gin/module/registry.hpp>

static std::unordered_map<std::string, std::function<size_t(Gin::Graph::Graph&)>> registry{};

std::unordered_map<std::string, std::function<size_t(Gin::Graph::Graph&)>>& Gin::Module::GetRegistry()
{
    return registry;
}
