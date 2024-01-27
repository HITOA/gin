#pragma once

#include <gin/graph/port.hpp>
#include <gin/graph/graphdef.hpp>
#include <gin/field/sampler.hpp>
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
    bool GraphPortSamplerCopier(std::any& value, Port& port) {
        if (!(((int)port.GetType().type & (int)PortType::Field) || (port.GetType().type == PortType::Dynamic)))
            return false;

        value = *((Field::Sampler<T>*)port.GetProperty());
        return true;
    }

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

		std::vector<std::pair<GraphId, GraphId>>& GetLinks() { return links; };

	private:
		std::any value{};
		std::function<bool(std::any&, Port&)> copier{};
		std::function<void*(std::any&)> caster{};
		std::vector<std::pair<GraphId, GraphId>> links{};
	};

    template<>
    inline void GraphPort::SetValue<Field::Sampler<Math::Scalar>>(Field::Sampler<Math::Scalar> value) {
        SetType(GetPortTypeInfo<Field::Sampler<Math::Scalar>>());
        copier = GraphPortSamplerCopier<Math::Scalar>;
        caster = GraphPortCaster<Field::Sampler<Math::Scalar>>;
        this->value = value;
    }
    template<>
    inline void GraphPort::SetValue<Field::Sampler<Math::Vector2>>(Field::Sampler<Math::Vector2> value) {
        SetType(GetPortTypeInfo<Field::Sampler<Math::Vector2>>());
        copier = GraphPortSamplerCopier<Math::Vector2>;
        caster = GraphPortCaster<Field::Sampler<Math::Vector2>>;
        this->value = value;
    }
    template<>
    inline void GraphPort::SetValue<Field::Sampler<Math::Vector3>>(Field::Sampler<Math::Vector3> value) {
        SetType(GetPortTypeInfo<Field::Sampler<Math::Vector3>>());
        copier = GraphPortSamplerCopier<Math::Vector3>;
        caster = GraphPortCaster<Field::Sampler<Math::Vector3>>;
        this->value = value;
    }
    template<>
    inline void GraphPort::SetValue<Field::Sampler<Math::Vector4>>(Field::Sampler<Math::Vector4> value) {
        SetType(GetPortTypeInfo<Field::Sampler<Math::Vector4>>());
        copier = GraphPortSamplerCopier<Math::Vector4>;
        caster = GraphPortCaster<Field::Sampler<Math::Vector4>>;
        this->value = value;
    }
    template<>
    inline void GraphPort::SetValue<Field::Sampler<Field::Dynamic>>(Field::Sampler<Field::Dynamic> value) {
        SetType(GetPortTypeInfo<Field::Sampler<Field::Dynamic>>());
        copier = GraphPortSamplerCopier<Field::Dynamic>;
        caster = GraphPortCaster<Field::Sampler<Field::Dynamic>>;
        this->value = value;
    }

}
