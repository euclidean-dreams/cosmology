#pragma once

#include "axioms.h"
#include "luon.h"


namespace cosmology {
class DamperPalette : public Name {
private:
    uptr<Harmony> harmony;
    float hue_mod = 0.1;
    float hue = 0;

public:
    DamperPalette(uptr<Harmony> harmony) :
        harmony{mv(harmony)} {
    }


    HSLColor get_color() {
        return {scint(hue), 100, 50};
    }

    void step() {
        float harmony_flux = 0;
        for (auto &luon: *harmony->luons) {
            harmony_flux += std::abs(luon->delta);
        }
        hue_mod += harmony_flux / 777;
        if (hue_mod >= 0.1) {
            hue_mod -= hue_mod / 10;
        }
        hue += hue_mod;
    }
};
}
