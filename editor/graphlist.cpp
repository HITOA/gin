#include "graphlist.hpp"

static std::vector<std::shared_ptr<GraphListEntry>> graphList{};

std::vector<std::shared_ptr<GraphListEntry>>& GetGraphList()
{
	return graphList;
}
