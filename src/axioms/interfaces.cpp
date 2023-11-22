#include "interfaces.h"

namespace cosmology {

up<std::thread> Circlet::begin(std::unique_ptr<Circlet> circlet) {
    auto thread = mkup<std::thread>(circle, mv(circlet));
    return thread;
}

void Circlet::circle(std::unique_ptr<Circlet> circlet) {
    while (!circlet->finished()) {
        auto cycle_start_time = get_current_time();
        circlet->activate();
        auto total_cycle_time = get_elapsed_time(cycle_start_time);
        if (circlet->get_tick_interval() > total_cycle_time) {
            sleep(circlet->get_tick_interval() - total_cycle_time);
        }
    }
}

}
