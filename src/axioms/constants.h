#pragma once

#include <cstdint>

namespace cosmology {
extern int OBSERVATION_WIDTH;
extern int OBSERVATION_HEIGHT;
extern uint64_t WORLD_SEED[4];

#ifdef threads_found
extern int LUON_COUNT;
extern int THREAD_COUNT;
extern int RENDER_TIMEOUT_MICROSECONDS;
#endif
}
