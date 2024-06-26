#pragma once

#include <string>
#include <gin/graph/porttype.hpp>

namespace Gin::Graph {

	class Port {
	public:
		Port(PortTypeInfo type, const std::string& name) : type{ type }, name{ name } {};
		virtual ~Port() {};

		virtual bool Match(Port& port) = 0;
		virtual bool CopyFrom(Port& port) = 0;
		virtual void* GetProperty() = 0;
        virtual void Clear() = 0;

		inline PortTypeInfo GetType() { return type; };
		inline void SetType(PortTypeInfo type) { this->type = type; };
		inline std::string& GetName() { return name; };

	private:
		PortTypeInfo type{};
		std::string name{};
		void* property{ nullptr };
	};
}
