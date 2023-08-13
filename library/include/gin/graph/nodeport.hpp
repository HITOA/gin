#pragma once

#include <gin/graph/port.hpp>
#include <Eigen/Core>

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

	//Number

	template<>
	class NodePort<int> : public Port {
	public:
		NodePort(const std::string& name, int& property) : Port(GetPortTypeInfo<int>(), name), property{ &property } {};

		virtual bool Match(Port& port) final {
			return port.GetType().type == PortType::Number;
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
			return port.GetType().type == PortType::Number;
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
			return port.GetType().type == PortType::Number;
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
	};

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
	};

	template<typename T>
	inline std::shared_ptr<Port> CreateNodePort(const std::string& name, T& property) {
		return std::make_shared<NodePort<T>>(name, property);
	}

}
