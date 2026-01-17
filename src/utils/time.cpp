#include "utils/time.h"

#include <SDL3/SDL_timer.h>

namespace Time {
	namespace {
		uint64_t startTime = 0.0;
		uint64_t endTime = 0.0;
		uint64_t deltaTime = 0.0;
	}

	void Init() {
		startTime = SDL_GetTicks();
	}

	void Update() {
		endTime = SDL_GetTicks();
		deltaTime = endTime - startTime;
		startTime = endTime;
	}

	float GetDeltaTimeSec() {
		return static_cast<float>(deltaTime) / 1000.0f;
	}

	uint64_t GetDeltaTimeMs() {
		return deltaTime;
	}
}
