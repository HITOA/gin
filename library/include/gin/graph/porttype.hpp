#pragma once

#include <typeinfo>
#include <gin/math/type.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Graph {

	enum class PortType {
		Object = 1,
		Scalar = 2,
		Vector2 = 4,
		Vector3 = 8,
		Vector4 = 16,
		Field = 32,
        Dynamic = 64
	};

	template<typename T>
	inline PortType GetPortType() {
		return PortType::Object;
	}

	template<typename T>
	inline const char* GetPortTypeShortName() {
		return "Obj";
	}

	template<typename T>
	inline const char* GetPortTypeFullName() {
		return "Object";
	}

	//Number
	template<>
	inline PortType GetPortType<Field::Sampler<Math::Scalar>>() {
		return (PortType)((int)PortType::Scalar + (int)PortType::Field);
	}
	template<>
	inline PortType GetPortType<Field::Sampler<Math::Vector2>>() {
        return (PortType)((int)PortType::Vector2 + (int)PortType::Field);
	}
	template<>
	inline PortType GetPortType<Field::Sampler<Math::Vector3>>() {
        return (PortType)((int)PortType::Vector3 + (int)PortType::Field);
	}
    template<>
    inline PortType GetPortType<Field::Sampler<Math::Vector4>>() {
        return (PortType)((int)PortType::Vector4 + (int)PortType::Field);
    }

	template<>
	inline const char* GetPortTypeShortName<Field::Sampler<Math::Scalar>>() {
		return "Num";
	}
	template<>
	inline const char* GetPortTypeShortName<Field::Sampler<Math::Vector2>>() {
		return "Vec2";
	}
	template<>
	inline const char* GetPortTypeShortName<Field::Sampler<Math::Vector3>>() {
		return "Vec3";
	}
    template<>
    inline const char* GetPortTypeShortName<Field::Sampler<Math::Vector4>>() {
        return "Vec4";
    }

	template<>
	inline const char* GetPortTypeFullName<Field::Sampler<Math::Scalar>>() {
		return "Float";
	}
	template<>
	inline const char* GetPortTypeFullName<Field::Sampler<Math::Vector2>>() {
		return "Vector2";
	}
	template<>
	inline const char* GetPortTypeFullName<Field::Sampler<Math::Vector3>>() {
		return "Vector3";
	}
    template<>
    inline const char* GetPortTypeFullName<Field::Sampler<Math::Vector4>>() {
        return "Vector4";
    }

    /*
	//Vector 2
	template<>
	inline PortType GetPortType<Eigen::Vector2<int>>() {
		return PortType::Vector2;
	}
	template<>
	inline PortType GetPortType<Eigen::Vector2<float>>() {
		return PortType::Vector2;
	}
	template<>
	inline PortType GetPortType<Eigen::Vector2<double>>() {
		return PortType::Vector2;
	}

	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector2<int>>() {
		return "Vec2";
	}
	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector2<float>>() {
		return "Vec2";
	}
	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector2<double>>() {
		return "Vec2";
	}

	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector2<int>>() {
		return "Vec2<Int>";
	}
	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector2<float>>() {
		return "Vec2<Float>";
	}
	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector2<double>>() {
		return "Vec2<Double>";
	}

	//Vector 3
	template<>
	inline PortType GetPortType<Eigen::Vector3<int>>() {
		return PortType::Vector3;
	}
	template<>
	inline PortType GetPortType<Eigen::Vector3<float>>() {
		return PortType::Vector3;
	}
	template<>
	inline PortType GetPortType<Eigen::Vector3<double>>() {
		return PortType::Vector3;
	}

	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector3<int>>() {
		return "Vec3";
	}
	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector3<float>>() {
		return "Vec3";
	}
	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector3<double>>() {
		return "Vec3";
	}

	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector3<int>>() {
		return "Vec3<Int>";
	}
	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector3<float>>() {
		return "Vec3<Float>";
	}
	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector3<double>>() {
		return "Vec3<Double>";
	}

	//Color

	template<>
	inline PortType GetPortType<Eigen::Vector4<float>>() {
		return PortType::Color;
	}

	template<>
	inline const char* GetPortTypeShortName<Eigen::Vector4<float>>() {
		return "Color";
	}

	template<>
	inline const char* GetPortTypeFullName<Eigen::Vector4<float>>() {
		return "Color";
	}*/

	//Spatial Number
	/*template<>
	inline PortType GetPortType<Spatial::Spatial<int>>() {
		return (PortType)((int)PortType::Number + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<float>>() {
		return (PortType)((int)PortType::Number + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<double>>() {
		return (PortType)((int)PortType::Number + (int)PortType::Spatial);
	}

	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<int>>() {
		return "Num";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<float>>() {
		return "Num";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<double>>() {
		return "Num";
	}

	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<int>>() {
		return "Spatial<Int>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<float>>() {
		return "Spatial<Float>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<double>>() {
		return "Spatial<Double>";
	}

	//Spatial Vector2
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector2<int>>>() {
		return (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector2<float>>>() {
		return (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector2<double>>>() {
		return (PortType)((int)PortType::Vector2 + (int)PortType::Spatial);
	}

	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector2<int>>>() {
		return "Vec2";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector2<float>>>() {
		return "Vec2";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector2<double>>>() {
		return "Vec2";
	}

	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector2<int>>>() {
		return "Spatial<Vec2<Int>>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector2<float>>>() {
		return "Spatial<Vec2<Float>>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector2<double>>>() {
		return "Spatial<Vec2<Double>>";
	}

	//Spatial Vector3
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector3<int>>>() {
		return (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector3<float>>>() {
		return (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
	}
	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector3<double>>>() {
		return (PortType)((int)PortType::Vector3 + (int)PortType::Spatial);
	}

	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector3<int>>>() {
		return "Vec3";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector3<float>>>() {
		return "Vec3";
	}
	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector3<double>>>() {
		return "Vec3";
	}

	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector3<int>>>() {
		return "Spatial<Vec3<Int>>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector3<float>>>() {
		return "Spatial<Vec3<Float>>";
	}
	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector3<double>>>() {
		return "Spatial<Vec3<Double>>";
	}

	//Spatial Color

	template<>
	inline PortType GetPortType<Spatial::Spatial<Eigen::Vector4<float>>>() {
		return (PortType)((int)PortType::Color + (int)PortType::Spatial);
	}

	template<>
	inline const char* GetPortTypeShortName<Spatial::Spatial<Eigen::Vector4<float>>>() {
		return "Color";
	}

	template<>
	inline const char* GetPortTypeFullName<Spatial::Spatial<Eigen::Vector4<float>>>() {
		return "Spatial<Color>";
	}*/

	//TypeInfo

	struct PortTypeInfo {
		std::reference_wrapper<const std::type_info> info{ typeid(void) };
		PortType type{};
		const char* shortName{};
		const char* fullName{};

		PortTypeInfo(const std::type_info& info, PortType type, const char* shortName, const char* fullName) : info{ info }, type{ type }, shortName{ shortName }, fullName{ fullName } {};
		PortTypeInfo() : info{ typeid(void) }, type{ PortType::Object }, shortName{ GetPortTypeShortName<void>() } {};

		friend inline bool operator==(const PortTypeInfo& v1, const PortTypeInfo& v2) {
			return v1.info.get() == v2.info.get();
		}

		friend inline bool operator!=(const PortTypeInfo& v1, const PortTypeInfo& v2) {
			return !(v1 == v2);
		}
	};

	template<typename T>
	PortTypeInfo GetPortTypeInfo() {
		return PortTypeInfo{ typeid(T), GetPortType<T>(), GetPortTypeShortName<T>(), GetPortTypeFullName<T>() };
	}

}
