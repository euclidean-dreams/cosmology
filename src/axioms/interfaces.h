#pragma once

#include <thread>
#include "macros.h"
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

    static up<std::thread> begin(up<Circlet> circlet);

    static void circle(up<Circlet> circlet);
};

}
