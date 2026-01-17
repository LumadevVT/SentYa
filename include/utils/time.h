#pragma once

#include <cstdint>

namespace Time {
	void Init();

	void Update();

	float GetDeltaTimeSec();
	uint64_t GetDeltaTimeMs();
}
