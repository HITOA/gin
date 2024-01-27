#pragma once

#include <gin/graph/port.hpp>
#include <memory>
#include <gin/field/sampler.hpp>

namespace Gin::Graph {

	template<typename T>
	class NodePort : public Port {
	public:
		NodePort(const std::string& name, T& property) : Port(GetPortTypeInfo<T>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return GetType() == port.GetType();
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			*property = *((T*)port.GetProperty());
			return true;
		}
		virtual void* GetProperty() final { return property; };
	public:
		T* property{ nullptr };
	};

    template<typename U>
    class NodePort<Field::Sampler<U>> : public Port {
    public:
        NodePort(const std::string& name, Field::Sampler<U>& property) :
            Port(GetPortTypeInfo<Field::Sampler<U>>(), name), property{ &property } {};

        virtual bool Match(Port& port) final {
            return ((int)port.GetType().type & (int)PortType::Field) || (port.GetType().type == PortType::Dynamic);
        }
        virtual bool CopyFrom(Port& port) final {
            if (!Match(port))
                return false;

            *property = *((Field::Sampler<U>*)port.GetProperty());

            return true;
        }
        virtual void* GetProperty() final { return property; };
    public:
        Field::Sampler<U>* property{ nullptr };
    };

	//Number

	/*template<>
	class NodePort<int> : public Port {
	public:
		NodePort(const std::string& name, int& property) : Port(GetPortTypeInfo<int>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Scalar;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((int*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<float>()) {
				*property = (int)*((float*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<double>()) {
				*property = (int)*((double*)port.GetProperty());
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		int* property{ nullptr };
	};

	template<>
	class NodePort<float> : public Port {
	public:
		NodePort(const std::string& name, float& property) : Port(GetPortTypeInfo<float>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Scalar;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((float*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<int>()) {
				*property = (float)*((int*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<double>()) {
				*property = (float)*((double*)port.GetProperty());
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		float* property{ nullptr };
	};

	template<>
	class NodePort<double> : public Port {
	public:
		NodePort(const std::string& name, double& property) : Port(GetPortTypeInfo<double>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Scalar;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((double*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<int>()) {
				*property = (double)*((int*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<float>()) {
				*property = (double)*((float*)port.GetProperty());
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		double* property{ nullptr };
	};*

	//Vector2

	template<>
	class NodePort<Eigen::Vector2<int>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector2<int>& property) : Port(GetPortTypeInfo<Eigen::Vector2<int>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector2;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector2<int>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<float>>()) {
				property->x() = (int)((Eigen::Vector2<float>*)port.GetProperty())->x();
				property->y() = (int)((Eigen::Vector2<float>*)port.GetProperty())->y();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<double>>()) {
				property->x() = (int)((Eigen::Vector2<double>*)port.GetProperty())->x();
				property->y() = (int)((Eigen::Vector2<double>*)port.GetProperty())->y();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector2<int>* property{ nullptr };
	};

	template<>
	class NodePort<Eigen::Vector2<float>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector2<float>& property) : Port(GetPortTypeInfo<Eigen::Vector2<float>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector2;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector2<float>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<int>>()) {
				property->x() = (float)((Eigen::Vector2<int>*)port.GetProperty())->x();
				property->y() = (float)((Eigen::Vector2<int>*)port.GetProperty())->y();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<double>>()) {
				property->x() = (float)((Eigen::Vector2<double>*)port.GetProperty())->x();
				property->y() = (float)((Eigen::Vector2<double>*)port.GetProperty())->y();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector2<float>* property{ nullptr };
	};

	template<>
	class NodePort<Eigen::Vector2<double>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector2<double>& property) : Port(GetPortTypeInfo<Eigen::Vector2<double>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector2;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector2<double>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<int>>()) {
				property->x() = (double)((Eigen::Vector2<int>*)port.GetProperty())->x();
				property->y() = (double)((Eigen::Vector2<int>*)port.GetProperty())->y();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector2<float>>()) {
				property->x() = (double)((Eigen::Vector2<float>*)port.GetProperty())->x();
				property->y() = (double)((Eigen::Vector2<float>*)port.GetProperty())->y();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector2<double>* property{ nullptr };
	};

	//Vector3

	template<>
	class NodePort<Eigen::Vector3<int>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector3<int>& property) : Port(GetPortTypeInfo<Eigen::Vector3<int>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector3;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector3<int>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<float>>()) {
				property->x() = (int)((Eigen::Vector3<float>*)port.GetProperty())->x();
				property->y() = (int)((Eigen::Vector3<float>*)port.GetProperty())->y();
				property->z() = (int)((Eigen::Vector3<float>*)port.GetProperty())->z();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<double>>()) {
				property->x() = (int)((Eigen::Vector3<double>*)port.GetProperty())->x();
				property->y() = (int)((Eigen::Vector3<double>*)port.GetProperty())->y();
				property->z() = (int)((Eigen::Vector3<double>*)port.GetProperty())->z();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector3<int>* property{ nullptr };
	};

	template<>
	class NodePort<Eigen::Vector3<float>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector3<float>& property) : Port(GetPortTypeInfo<Eigen::Vector3<float>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector3;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector3<float>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<int>>()) {
				property->x() = (float)((Eigen::Vector3<int>*)port.GetProperty())->x();
				property->y() = (float)((Eigen::Vector3<int>*)port.GetProperty())->y();
				property->z() = (float)((Eigen::Vector3<int>*)port.GetProperty())->z();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<double>>()) {
				property->x() = (float)((Eigen::Vector3<double>*)port.GetProperty())->x();
				property->y() = (float)((Eigen::Vector3<double>*)port.GetProperty())->y();
				property->z() = (float)((Eigen::Vector3<double>*)port.GetProperty())->z();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector3<float>* property{ nullptr };
	};

	template<>
	class NodePort<Eigen::Vector3<double>> : public Port {
	public:
		NodePort(const std::string& name, Eigen::Vector3<double>& property) : Port(GetPortTypeInfo<Eigen::Vector3<double>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Vector3;
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (GetType() == port.GetType()) {
				*property = *((Eigen::Vector3<double>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<int>>()) {
				property->x() = (double)((Eigen::Vector3<int>*)port.GetProperty())->x();
				property->y() = (double)((Eigen::Vector3<int>*)port.GetProperty())->y();
				property->z() = (double)((Eigen::Vector3<int>*)port.GetProperty())->z();
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Eigen::Vector3<float>>()) {
				property->x() = (double)((Eigen::Vector3<float>*)port.GetProperty())->x();
				property->y() = (double)((Eigen::Vector3<float>*)port.GetProperty())->y();
				property->z() = (double)((Eigen::Vector3<float>*)port.GetProperty())->z();
				return true;
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Eigen::Vector3<double>* property{ nullptr };
	};*/

	//Spatial Number
    /*
	template<>
	class NodePort<Spatial::Spatial<int>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<int>& property) : Port(GetPortTypeInfo<Spatial::Spatial<int>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Number + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<int>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<float>>()) {
				Spatial::Spatial<float>* spatial = (Spatial::Spatial<float>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (int)((*spatial)[i]);
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<double>>()) {
				Spatial::Spatial<double>* spatial = (Spatial::Spatial<double>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (int)((*spatial)[i]);
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<int>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<float>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<float>& property) : Port(GetPortTypeInfo<Spatial::Spatial<float>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Number + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<float>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<int>>()) {
				Spatial::Spatial<int>* spatial = (Spatial::Spatial<int>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (float)((*spatial)[i]);
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<double>>()) {
				Spatial::Spatial<double>* spatial = (Spatial::Spatial<double>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (float)((*spatial)[i]);
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<float>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<double>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<double>& property) : Port(GetPortTypeInfo<Spatial::Spatial<double>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Number + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<double>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<int>>()) {
				Spatial::Spatial<int>* spatial = (Spatial::Spatial<int>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (double)((*spatial)[i]);
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<float>>()) {
				Spatial::Spatial<float>* spatial = (Spatial::Spatial<float>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = (double)((*spatial)[i]);
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<double>* property{ nullptr };
	};

	//Spatial Vector2

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector2<int>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector2<int>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<int>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector2<int>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<float>>>()) {
				Spatial::Spatial<Eigen::Vector2<float>>* spatial = (Spatial::Spatial<Eigen::Vector2<float>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<int>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<double>>>()) {
				Spatial::Spatial<Eigen::Vector2<double>>* spatial = (Spatial::Spatial<Eigen::Vector2<double>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<int>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector2<int>>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector2<float>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector2<float>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<float>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector2<float>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<int>>>()) {
				Spatial::Spatial<Eigen::Vector2<int>>* spatial = (Spatial::Spatial<Eigen::Vector2<int>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<float>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<double>>>()) {
				Spatial::Spatial<Eigen::Vector2<double>>* spatial = (Spatial::Spatial<Eigen::Vector2<double>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<float>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector2<float>>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector2<double>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector2<double>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<double>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector2<double>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<int>>>()) {
				Spatial::Spatial<Eigen::Vector2<int>>* spatial = (Spatial::Spatial<Eigen::Vector2<int>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<double>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector2<float>>>()) {
				Spatial::Spatial<Eigen::Vector2<float>>* spatial = (Spatial::Spatial<Eigen::Vector2<float>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<double>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector2<double>>* property{ nullptr };
	};

	//Spatial Vector3

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector3<int>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector3<int>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<int>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector3<int>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<float>>>()) {
				Spatial::Spatial<Eigen::Vector3<float>>* spatial = (Spatial::Spatial<Eigen::Vector3<float>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<int>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<double>>>()) {
				Spatial::Spatial<Eigen::Vector3<double>>* spatial = (Spatial::Spatial<Eigen::Vector3<double>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<int>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector3<int>>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector3<float>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector3<float>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<float>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector3<float>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<int>>>()) {
				Spatial::Spatial<Eigen::Vector3<int>>* spatial = (Spatial::Spatial<Eigen::Vector3<int>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<float>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<double>>>()) {
				Spatial::Spatial<Eigen::Vector3<double>>* spatial = (Spatial::Spatial<Eigen::Vector3<double>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<float>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector3<float>>* property{ nullptr };
	};

	template<>
	class NodePort<Spatial::Spatial<Eigen::Vector3<double>>> : public Port {
	public:
		NodePort(const std::string& name, Spatial::Spatial<Eigen::Vector3<double>>& property) : Port(GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<double>>>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
		}
		virtual bool CopyFrom(Port& port) final {
			if (!Match(port))
				return false;

			if (port.GetType() == GetType()) {
				*property = *((Spatial::Spatial<Eigen::Vector3<double>>*)port.GetProperty());
				return true;
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<int>>>()) {
				Spatial::Spatial<Eigen::Vector3<int>>* spatial = (Spatial::Spatial<Eigen::Vector3<int>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<double>();
			}

			if (port.GetType() == GetPortTypeInfo<Spatial::Spatial<Eigen::Vector3<float>>>()) {
				Spatial::Spatial<Eigen::Vector3<float>>* spatial = (Spatial::Spatial<Eigen::Vector3<float>>*)port.GetProperty();
				size_t max = property->GetWidth() * property->GetHeight() * property->GetDepth();
				for (size_t i = 0; i < max; ++i)
					(*property)[i] = ((*spatial)[i]).cast<double>();
			}

			return false;
		}
		virtual void* GetProperty() final { return property; };
	public:
		Spatial::Spatial<Eigen::Vector3<double>>* property{ nullptr };
	};*/

	template<typename T>
	inline std::shared_ptr<Port> CreateNodePort(const std::string& name, T& property) {
		return std::make_shared<NodePort<T>>(name, property);
	}

}
