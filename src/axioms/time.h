#pragma once

#include <chrono>
#include "macros.h"

namespace cosmology {

uint64_t get_current_time() {
    auto ra_time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());
    return ra_time.time_since_epoch().count();
}

uint64_t get_elapsed_time(uint64_t initial_time) {
    return get_current_time() - initial_time;
}

void sleep(uint64_t microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

class Timer {
private:
    uint64_t start_time{get_current_time()};

public:
    uint64_t get_time() const {
        return get_elapsed_time(start_time);
    }
};

}
