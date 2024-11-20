#pragma once

#include "axioms.h"
#include "impression.h"
#include "palettes/centrifugal_palette.h"

using namespace cosmology;
namespace obsidian {


class Refraction : public Name {
public:
    Luon &luon;
    Point origin;
    CentrifugalPalette & palette;
    bool direction;

    Refraction(Luon &luon, Point origin, CentrifugalPalette &palette)
            : luon{luon},
              origin{origin},
              palette{palette},
              direction{true} {

    }

    void paint(Lattice &lattice) {
        if (luon.energy > 0.001) {
            auto size = luon.smooth_log * 9 + luon.energy * 3;
            auto start_index = embind(0, origin.x - size / 2, OBSERVATION_WIDTH);
            auto end_index = embind(0, origin.x + size / 2, OBSERVATION_WIDTH);
            auto current_color = palette.get_color();
            current_color.lightness = cyclic_embind(0, luon.energy * 333, 100);
            for (int i = start_index; i < end_index; i++) {
                lattice.set_pith(i, origin.y, Pith{current_color.convert_to_rgb()});
            }
        }
    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 333 * luon.log_energy * MOVEMENT;
        if (direction) {
            origin.x += distance;
        } else {
            origin.x -= distance;
        }
        if (std::abs(luon.delta) > 0.05) {
            direction = !direction;
        }
    }
};

class Obsidian : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Refraction>> refractions;
    uptr<CentrifugalPalette> palette;

public:
    Obsidian(Psyche &psyche)
            : psyche{psyche},
              refractions{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        palette = mkuptr<CentrifugalPalette>(mv(harmony), 33);
        harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto refraction = mkuptr<Refraction>(*luon, Point{x, y}, *palette);
            refractions.push_back(mv(refraction));
        }
    }

    uptr<Lattice> experience() override {
        palette->step();
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        for (auto &refraction: refractions) {
            refraction->paint(*lattice);
            refraction->move();
        }
        return lattice;
    }
};

}
