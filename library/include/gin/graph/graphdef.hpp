#pragma once

#define GRAPH_ID_MAX (4294967295U)
#define GRAPH_INPUT_NODE_ID GRAPH_ID_MAX
#define GRAPH_OUTPUT_NODE_ID (GRAPH_ID_MAX - 1)
#define GRAPH_MAX_PORT_ID (GRAPH_ID_MAX - 2)

namespace Gin::Graph {
    typedef unsigned int GraphId;
}