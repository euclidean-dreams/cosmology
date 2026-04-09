#pragma once

#include "axioms.h"
#include "impression.h"
#include "palettes/damper_palette.h"

using namespace cosmology;


namespace ambiance {
int LANTERN_COUNT = 20;
int UPPER_PARTIAL_COUNT = 8;


class Lantern : public Name {
private:
    uptr<Harmony> harmony;
    Point origin;
    float radiance = 0;
    float damper = 0;
    float damper_delta = 0.01;

public:
    Lantern(Psyche &psyche, Luon &root_luon, Point origin)
        : origin{origin} {
        vect<int> luon_indices{};
        for (int upper_partial_index = 1; upper_partial_index < UPPER_PARTIAL_COUNT; upper_partial_index++) {
            auto luon_index = upper_partial_index * (root_luon.index + 1);
            if (luon_index < psyche.size()) {
                luon_indices.push_back(luon_index);
            }
        }
        harmony = psyche.create_harmony(luon_indices);
    }

    void show(Lattice &lattice, DamperPalette &palette) {
        float harmony_energy = 0;
        for (auto &luon: *harmony->luons) {
            harmony_energy += luon->smooth_log * 33;
        }
        radiance = embind_flt(0, radiance + harmony_energy - damper, 100);
        if (radiance >= 90) {
            damper_delta += 0.01;
        }
        if (radiance >= 10) {
            damper += damper_delta;
        }
        if (radiance <= 10) {
            damper -= damper_delta;
        }
        damper_delta -= 0.001;
        if (damper_delta < 0.01) {
            damper_delta = 0.01;
        }

        auto color = HSLColor{palette.get_color().hue, 100, scint(radiance / 2)};
        lattice.set_pith(scint(origin.x), scint(origin.y), Pith{color.convert_to_rgb(), 1});
    }
};


class Ambiance : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Lantern>> lanterns;
    uptr<DamperPalette> palette;

public:
    Ambiance(Psyche &psyche)
        : psyche{psyche} {
        vect<int> luon_indices{};
        luon_indices.reserve(LANTERN_COUNT);
        for (int i = 0; i < LANTERN_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        auto index = 0;
        for (auto &luon: *harmony->luons) {
            int x = index;
            int y = 0;
            auto lantern = mkuptr<Lantern>(psyche, *luon, Point{x, y});
            lanterns.push_back(mv(lantern));
            index++;
        }

        auto palette_harmony = psyche.create_harmony(luon_indices);
        palette = mkuptr<DamperPalette>(mv(palette_harmony));
    }

    uptr<Lattice> experience() override {
        auto background = HSLColor{0, 0, 0};
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background.convert_to_rgb()}, true);
        palette->step();
        for (auto &lantern: lanterns) {
            lantern->show(*lattice, *palette);
        }
        return lattice;
    }
};
}
