#pragma once

#include <chrono>
#include "macros.h"

namespace cosmology {

uint64_t get_current_time();

uint64_t get_elapsed_time(uint64_t initial_time);

void sleep(uint64_t microseconds);

class Timer {
private:
    uint64_t start_time{get_current_time()};

public:
    uint64_t get_time() const {
        return get_elapsed_time(start_time);
    }
};

}
