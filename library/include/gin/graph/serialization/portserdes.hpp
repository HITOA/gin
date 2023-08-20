#pragma once

#include <gin/graph/port.hpp>
#include <gin/graph/serialization/json.hpp>

namespace Gin::Graph::Serialization {

	class BasePortSerializer {
	public:
		virtual nlohmann::json Serialize(Port& port) = 0;
		virtual void Deserialize(Port& port, nlohmann::json serializedPort) = 0;
	};

	template<typename T>
	class PortSerializer : public BasePortSerializer {

	};

}
