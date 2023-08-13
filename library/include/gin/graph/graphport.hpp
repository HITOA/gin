#pragma once

#include <gin/graph/port.hpp>
#include <any>

namespace Gin::Graph {
	
	template<typename T>
	bool GraphPortCopier(std::any& value, Port& port) {
		if (port.GetType() != GetPortTypeInfo<T>())
			return false;

		value = *((T*)port.GetProperty());
		return true;
	};

	template<typename T>
	void* GraphPortCaster(std::any& value) {
		return (void*)(std::any_cast<T>(&value));
	}

	class GraphPort : public Port {
	public:
		GraphPort(const std::string& name) : Port(GetPortTypeInfo<int>(), name), value{0}, copier{GraphPortCopier<int>}, caster{GraphPortCaster<int>}, links{} {};

		virtual bool Match(Port& port) final {
			return GetType() == port.GetType();
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			return copier(value, port);
		}
		virtual void* GetProperty() final {
			return caster(value);
		};

		template<typename T>
		void SetValue(T value) {
			SetType(GetPortTypeInfo<T>());
			copier = GraphPortCopier<T>;
			caster = GraphPortCaster<T>;
			this->value = value;
		}

		std::vector<std::pair<size_t, size_t>>& GetLinks() { return links; };

	private:
		std::any value{};
		std::function<bool(std::any&, Port&)> copier{};
		std::function<void*(std::any&)> caster{};
		std::vector<std::pair<size_t, size_t>> links{};
	};

}
