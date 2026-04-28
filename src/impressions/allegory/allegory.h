#pragma once

#include "axioms.h"
#include "../impression.h"
#include "allegorion/allegorion.h"

using namespace cosmology;


namespace allegory {
class Allegory : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Harmony>> chorus;
    vect<uptr<Allegorion>> allegorions;
    uptr<DamperPalette> palette;

public:
    Allegory(Psyche &psyche)
        : psyche{psyche} {
        // harmony
        vect<int> luon_indices{};
        luon_indices.reserve(psyche.size());
        for (int fundamental = 1; fundamental < psyche.size(); fundamental++) {
            for (int partial = fundamental; partial < psyche.size(); partial += fundamental) {
                luon_indices.push_back(partial);
            }
            auto harmony = this->psyche.create_harmony(luon_indices);
            chorus.push_back(mv(harmony));
        }

        // allegorions
        auto index = 0;
        for (auto &allegorion_harmony: chorus) {
            int x = index;
            int y = 0;
            auto allegorion = mkuptr<Allegorion>(*allegorion_harmony, Point{x, y});
            allegorions.push_back(mv(allegorion));
            index++;
        }

        // palette
        auto palette_harmony = psyche.create_harmony(luon_indices);
        palette = mkuptr<DamperPalette>(mv(palette_harmony));
    }

    uptr<Lattice> experience() override {
        auto background = HSLColor{0, 0, 0};
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background.convert_to_rgb()}, true);
        palette->step();
        for (auto &allegorion: allegorions) {
            allegorion->sing(*lattice, *palette);
        }
        return lattice;
    }
};
}
