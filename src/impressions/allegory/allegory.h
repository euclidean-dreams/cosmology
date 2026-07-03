#pragma once

#include "axioms.h"
#include "../impression.h"
#include "allegorion/allegorion.h"

using namespace cosmology;


namespace allegory {
class Allegory : public Impression {
private:
    Psyche &psyche;
    int mode;
    vect<uptr<Harmony>> chorus;
    vect<uptr<Allegorion>> allegorions;
    uptr<CentrifugalPalette> palette;

public:
    Allegory(Psyche &psyche, int mode)
        : psyche{psyche},
          mode{mode} {
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
            int x = Randomizer::generate(OBSERVATION_WIDTH);
            int y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto &fundamental = *(*(chorus[index])->luons)[index];

            uptr<Allegorion> allegorion;
            if (mode == 0) {
                allegorion = mkuptr<Toroidaloids>(chorus, fundamental, Point{x, y}, HSLColor{0, 100, 50});
            } else if (mode == 1) {
                allegorion = mkuptr<Fireflies>(chorus, fundamental, Point{x, y}, HSLColor{0, 100, 50});
            } else {
                allegorion = mkuptr<Toroidaloids>(chorus, fundamental, Point{x, y}, HSLColor{0, 100, 50});
            }

            allegorions.push_back(mv(allegorion));
            index++;
        }

        // palette
        auto palette_harmony = psyche.create_harmony(luon_indices);
        palette = mkuptr<CentrifugalPalette>(mv(palette_harmony), 33);
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
