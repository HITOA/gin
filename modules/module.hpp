#pragma once

#include <base/base.hpp>
#include <signed/signed.hpp>

inline void Initialize() {
	InitializeBaseModule();
	InitializeSignedModule();
}

inline void Uninitialize() {
	UninitializeBaseModule();
	UninitializeSignedModule();
}