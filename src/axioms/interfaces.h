#pragma once

#include "paradigm.h"

namespace PROJECT_NAMESPACE {

class Name {
public:
    Name(const Name &) = delete;

    Name &operator=(const Name &) = delete;

protected:
    Name() = default;
};

}
