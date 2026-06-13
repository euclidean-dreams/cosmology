#pragma once

#include "axioms.h"

using namespace cosmology;


namespace allegory {
class Allegorion : public Name {
protected:
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

    virtual void sing(Lattice &lattice, CentrifugalPalette &palette) {
        float movement_magnitude = 0.01 + 0.01 * fundamental.energy;
        origin = Point::from_polar(origin, movement_magnitude, direction);
        direction += 0.01 * fundamental.delta;

        auto cardinal = toroidalize(origin);

        auto color_magnitude = fundamental.smooth_log * 33;
        color.hue = scint(color_magnitude) / 3 + palette.get_color().hue;
        color.hue %= HSL_HUE_MAX;
        auto saturation_magnitude = 33 + fundamental.energy * 333;
        color.saturation = embind_flt(0, saturation_magnitude, 100);;
        color.lightness = embind_flt(0, color_magnitude, 100);
        lattice.set_pith(scint(cardinal.x), scint(cardinal.y), Pith{color.convert_to_rgb(), 1});
    }

    Point toroidalize(Point origin) {
        auto theta = origin.y;
        auto circumference = scflt(OBSERVATION_HEIGHT);
        Point cardinal{
            cyclic_embind_flt(0, origin.x, scflt(OBSERVATION_WIDTH)),
            scflt(circumference / 2 * std::cos(theta / 2 + M_PI) + circumference / 2)
        };
        return cardinal;
    }
};

class Firefly : public Allegorion {
public:
    Firefly(vect<uptr<Harmony>> &chorus, Luon &fundamental, Point origin, HSLColor color)
        : Allegorion(chorus, fundamental, origin, color) {
    }

    void sing(Lattice &lattice, CentrifugalPalette &palette) override {
        float movement_magnitude = 0.01 + 0.01 * fundamental.energy;
        origin = Point::from_polar(origin, movement_magnitude, direction);
        direction += 0.01 * fundamental.delta;

        auto cardinal = toroidalize(origin);

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
