#include <gin/module/registry.hpp>

static std::map<std::string, std::function<size_t(Gin::Graph::Graph&, std::string)>> registry{};

std::map<std::string, std::function<size_t(Gin::Graph::Graph&, std::string)>>& Gin::Module::GetNodeRegistry()
{
    return registry;
}
