#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;

namespace knit {
class Stitch : public Name {
public:
    sptr<Vertex> vertex;
    Luon &luon;

    Stitch(sptr<Vertex> vertex, Luon &luon) :
        vertex{mv(vertex)},
        luon{luon} {
    }
};


class Knit : public Impression {
private:
    Psyche &psyche;
    uptr<Harmony> harmony;
    uptr<vect<uptr<Stitch>>> stitches;

public:
    Knit(Psyche &psyche)
        : psyche{psyche},
          harmony{},
          stitches{mkuptr<vect<uptr<Stitch>>>()} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            auto origin = Point{
                Randomizer::generate(OBSERVATION_WIDTH),
                Randomizer::generate(OBSERVATION_HEIGHT)
            };
            auto stitch = mkuptr<Stitch>(mksptr<Vertex>(origin), *luon);
            stitches->push_back(mv(stitch));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        uptr<vect<sptr<Edge>>> edges = mkuptr<vect<sptr<Edge>>>();
        auto last_vertex = mksptr<Vertex>(Point{
            OBSERVATION_WIDTH / 2,
            OBSERVATION_HEIGHT / 2
        });
        for (auto &stitch: *stitches) {
            if (stitch->luon.energy > 3) {
                edges->push_back(mv(mksptr<Edge>(
                    last_vertex,
                    stitch->vertex
                )));
                last_vertex = stitch->vertex;
            }
        }
        auto polygon = mksptr<Polygon>(mv(edges));
        sketch_polygon(*polygon, lattice);
        return lattice;
    }

    static void sketch_polygon(Polygon &polygon, uptr<Lattice> &lattice) {
        for (auto &edge: *polygon.edges) {
            sketch_line(edge->to_line(), lattice);
        }
    }

    static void sketch_line(Line line, uptr<Lattice> &lattice) {
        int steps = std::sqrt(std::pow(line.p2.x - line.p1.x, 2) + std::pow(line.p2.y - line.p1.y, 2));
        for (int i = 0; i <= steps; i++) {
            float ratio = scflt(i) / scflt(steps);
            int x = scint((1 - ratio) * line.p1.x + ratio * line.p2.x);
            int y = scint((1 - ratio) * line.p1.y + ratio * line.p2.y);
            lattice->set_pith(x, y, Pith{{0, 250, 200}, 2});
        }
    }
};
}
