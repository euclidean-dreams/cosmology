#pragma once

#include "axioms.h"
#include "impression.h"
#include "palettes/centrifugal_palette.h"

using namespace cosmology;


namespace watercolor {
class Splash : public Name {
public:
    Luon &luon;
    Point origin;
    float direction;
    lst<Point> loci;

    Splash(Luon &luon, Point origin)
        : luon{luon},
          origin{origin},
          direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)},
          loci{} {
    }

    void paint(Lattice &lattice, CentrifugalPalette &palette) {
        while (loci.size() > luon.smooth_log * 3 * CHAOS) {
            loci.pop_front();
        }
        while (loci.size() < luon.smooth_log * 3 * CHAOS) {
            Point splat{
                Randomizer::generate(luon.smooth_log * TWIST * OBSERVATION_WIDTH / 99) * Randomizer::generate_sign(),
                Randomizer::generate(luon.smooth_log * TWIST * OBSERVATION_HEIGHT / 99) * Randomizer::generate_sign()
            };
            loci.push_front(splat);
        }
        int index = 0;
        for (auto &point: loci) {
            auto palette_color = palette.get_color();
            auto current_color = palette_color;
            auto hue_mod = Randomizer::generate(RESONANCE) * luon.delta;
            current_color.lightness = embind(0, 33 + luon.energy * 33, 100);
            current_color.saturation = embind(0, 50 + luon.energy * 3, 100);
            current_color.hue = cyclic_embind(0, current_color.hue + hue_mod, HSL_HUE_MAX);
            auto adjusted_point = Point{origin.x + point.x, origin.y + point.y};
            auto radius = MAGNITUDE * (luon.smooth_log * 9 - scflt(index) * 3);
            if (radius > MAGNITUDE * OBSERVATION_WIDTH / 9) {
                radius = MAGNITUDE * OBSERVATION_WIDTH / 9;
            }
            float twist = 2 * M_PI * Randomizer::generate_proportion();
            lattice.set_pith(adjusted_point.x, adjusted_point.y,
                             Pith{current_color.convert_to_rgb(), radius, twist});
            point.x += Randomizer::generate(OBSERVATION_WIDTH / 333) * Randomizer::generate_sign();
            point.y += Randomizer::generate(OBSERVATION_WIDTH / 333) * Randomizer::generate_sign();
            index++;
        }
    }

    void move() {
        float distance = std::abs(luon.delta) * MOVEMENT;
        origin = Point::from_polar(origin, distance, direction);
        if (origin.x < 0) {
            origin.x = 0;
            direction += M_PI * Randomizer::generate_proportion() + M_PI / 2;
        } else if (origin.x >= OBSERVATION_WIDTH) {
            origin.x = OBSERVATION_WIDTH - 1;
            direction += M_PI * Randomizer::generate_proportion() + M_PI / 2;
        }
        if (origin.y < 0) {
            origin.y = 0;
            direction += M_PI * Randomizer::generate_proportion() + M_PI / 2;
        } else if (origin.y >= OBSERVATION_HEIGHT) {
            origin.y = OBSERVATION_HEIGHT - 1;
            direction += M_PI * Randomizer::generate_proportion() + M_PI / 2;
        }
        auto center_drift_direction = std::atan2(scflt(OBSERVATION_HEIGHT) / 2 - origin.y,
                                                 scflt(OBSERVATION_WIDTH) / 2 - origin.x);
        origin = Point::from_polar(origin, OBSERVATION_WIDTH / 2222, center_drift_direction);
    }
};

class Watercolor : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Splash>> splashes;
    uptr<CentrifugalPalette> palette;

public:
    Watercolor(Psyche &psyche)
        : psyche{psyche},
          splashes{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = OBSERVATION_WIDTH / 2;
            float y = OBSERVATION_HEIGHT / 2;
            auto splash = mkuptr<Splash>(*luon, Point{x, y});
            splashes.push_back(mv(splash));
        }

        harmony = this->psyche.create_harmony(luon_indices);
        palette = mkuptr<CentrifugalPalette>(mv(harmony), 333);
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        palette->step();
        for (auto &splash: splashes) {
            splash->move();
            splash->paint(*lattice, *palette);
        }
        return lattice;
    }
};
}
