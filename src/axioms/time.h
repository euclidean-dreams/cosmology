#pragma once

#include <chrono>
#include "macros.h"

namespace cosmology {

uint64_t get_current_time();

uint64_t get_elapsed_time(uint64_t initial_time);

void sleep(uint64_t microseconds);

}
