#pragma once

#include <thread>
#include "./time.h"

namespace cosmology {

class Name {
public:
    Name(const Name &) = delete;

    Name &operator=(const Name &) = delete;

protected:
    Name() = default;
};

class Circlet : public Name {
public:
    virtual ~Circlet() = default;

    virtual void activate() = 0;

    virtual bool finished() { return false; };

    virtual uint64_t get_tick_interval() = 0;

    static up<std::thread> begin(up<Circlet> circlet) {
        auto thread = mkup<std::thread>(circle, mv(circlet));
        return thread;
    }

    static void circle(up<Circlet> circlet) {
        while (!circlet->finished()) {
            auto cycle_start_time = get_current_time();
            circlet->activate();
            auto total_cycle_time = get_elapsed_time(cycle_start_time);
            if (circlet->get_tick_interval() > total_cycle_time) {
                sleep(circlet->get_tick_interval() - total_cycle_time);
            }
        }
    }
};

}
