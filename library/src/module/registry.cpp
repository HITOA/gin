#include <gin/module/registry.hpp>

static std::unordered_map<std::string, std::function<size_t(Gin::Graph::Graph&, std::string)>> registry{};

std::unordered_map<std::string, std::function<size_t(Gin::Graph::Graph&, std::string)>>& Gin::Module::GetNodeRegistry()
{
    return registry;
}
