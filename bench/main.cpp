#include <nanobench.h>
#include <module.hpp>
#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

int main() {
	Initialize();

	Gin::Graph::GraphContext ctx{};
	ctx.scale = 1.0;
	ctx.bounds.extent = Eigen::Vector3<double>{64, 64, 64};

	for (auto& entry : Gin::Module::GetNodeRegistry()) {
		Gin::Graph::Graph graph{};
		size_t nodeIdx = entry.second(graph, entry.first);
		graph.GetNode<Gin::Graph::Node>(nodeIdx)->Initialize(ctx);

		ankerl::nanobench::Bench().run(("262144 sample - " + entry.first), [&] {
			graph.GetNode<Gin::Graph::Node>(nodeIdx)->Execute(ctx);
		});
	}

	Uninitialize();
}
