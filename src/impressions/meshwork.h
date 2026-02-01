#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;


namespace meshwork {
class Glimmer : public Name {
public:
    Luon &luon;
    sptr<Vertex> vertex;

    Glimmer(Luon &luon, sptr<Vertex> vertex) :
        luon{luon},
        vertex{mv(vertex)} {
    }
};

class Meshwork : public Impression {
private:
    Psyche &psyche;
    uptr<Harmony> harmony;
    vect<uptr<Glimmer>> glimmers;
    uptr<CentrifugalPalette> palette;

public:
    Meshwork(Psyche &psyche)
        : psyche{psyche},
          glimmers{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = this->psyche.create_harmony(luon_indices);
        palette = mkuptr<CentrifugalPalette>(mv(harmony), 33);

        harmony = this->psyche.create_harmony(luon_indices);
        auto mesh_width = 50;
        auto x = 0;
        auto y = 0;
        for (auto &luon: *harmony->luons) {
            auto vertex = mksptr<Vertex>(Point{x, y});
            auto glimmer = mkuptr<Glimmer>(*luon, mv(vertex));
            glimmers.push_back(mv(glimmer));
            x++;
            x %= mesh_width;
            if (x == 0) {
                y++;
            }
        }
    }


    uptr<Lattice> experience() override {
        auto background = HSLColor{0, 0, 0};
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background.convert_to_rgb()}, false);
        for (auto &glimmer: glimmers) {
            auto x = scint(glimmer->vertex->origin.x);
            auto y = scint(glimmer->vertex->origin.y);
            auto magnitude = 450 * glimmer->luon.energy * MAGNITUDE;
            auto palette_color = palette->get_color();
            auto color = HSLColor(palette_color.hue, embind(50, magnitude, 100), embind(0, magnitude, 100));
            auto pith = Pith{color.convert_to_rgb(), 1};
            lattice->set_pith(x, y, pith);
        }
        palette->step();
        return lattice;
    }
};
}
