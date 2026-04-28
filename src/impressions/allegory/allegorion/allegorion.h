#pragma once

#include "axioms.h"

using namespace cosmology;


namespace allegory {
class Allegorion : public Name {
    Harmony &harmony;
    Point origin;
    float radiance = 0;
    float damper = 0;
    float damper_delta = 0.01;

public:
    Allegorion(Harmony &harmony, Point origin)
        : harmony{harmony},
          origin{origin} {
    }

    void sing(Lattice &lattice, DamperPalette &palette) {
        auto &fundamental = *(*harmony.luons)[0];

        // color
        auto unbound_radiance = radiance + harmony.energy() * scflt(std::log(fundamental.index + 1)) - damper;
        radiance = embind_flt(0, unbound_radiance, 100);
        if (radiance >= 90) {
            damper_delta += 0.0001;
        }
        if (radiance >= 10) {
            damper += damper_delta;
        }
        if (radiance <= 10) {
            damper -= damper_delta;
        }
        damper_delta -= 0.00001;
        if (damper_delta < 0.0001) {
            damper_delta = 0.0001;
        }


        auto color = HSLColor{palette.get_color().hue, 100, scint(radiance)};
        for (auto x = fundamental.index; x < OBSERVATION_WIDTH; x+= fundamental.index) {
            lattice.set_pith(x, scint(origin.y), Pith{color.convert_to_rgb(), 1});
        }
    }
};
}
