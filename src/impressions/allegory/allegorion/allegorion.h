#pragma once

#include "axioms.h"

using namespace cosmology;


namespace allegory {
class Allegorion : public Name {
    vect<uptr<Harmony>> &chorus;
    Luon &fundamental;
    HSLColor color;
    float hue_augment;
    Point origin;
    float direction;

public:
    Allegorion(vect<uptr<Harmony>> &chorus, Luon &fundamental, Point origin, HSLColor color)
        : chorus{chorus},
          fundamental{fundamental},
          color{color},
          origin{origin},
          direction{scflt(2 * M_PI * Randomizer::generate_proportion())} {
    }

    void sing(Lattice &lattice, CentrifugalPalette &palette) {
        float movement_magnitude = 0.01 + 0.01 * fundamental.energy;
        origin = Point::from_polar(origin, movement_magnitude, direction);
        direction += 0.01 * fundamental.delta;

        auto theta = origin.x;
        auto phi = origin.y;
        float circumference = OBSERVATION_WIDTH;
        float depth = OBSERVATION_HEIGHT;
        Point cardinal{
            scflt(circumference / 2 * std::cos(theta / 2 + M_PI) + circumference / 2),
            scflt(depth / 2 * std::cos(phi / 2 + M_PI) + depth / 2)
        };

        auto color_magnitude = fundamental.smooth_log * 33;
        color.hue = scint(color_magnitude) / 3 + palette.get_color().hue;
        color.hue %= HSL_HUE_MAX;
        auto saturation_magnitude = 33 + fundamental.energy * 333;
        color.saturation = embind_flt(0, saturation_magnitude, 100);;
        color.lightness = embind_flt(0, color_magnitude, 100);
        lattice.set_pith(scint(cardinal.x), scint(cardinal.y), Pith{color.convert_to_rgb(), 1});
    }
};
}
