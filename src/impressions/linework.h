#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;

namespace linework {
class Linework : public Impression {
private:
    Psyche &psyche;
    uptr<Harmony> harmony;

public:
    Linework(Psyche &psyche)
        : psyche{psyche} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = this->psyche.create_harmony(luon_indices);
    }


    uptr<Mesh> draw_flower() {
        uptr<vect<sptr<Polygon>>> polygons = mkuptr<vect<sptr<Polygon>>>();
        auto center_screen = Point{OBSERVATION_WIDTH / 2, OBSERVATION_HEIGHT / 2};
        float radius_augment = 3;
        auto layers = 3;
        auto petal_count = 33;
        auto luon_base_index = 1;
        for (auto i = 0; i < layers; i++) {
            for (auto j = 0; j < petal_count; j++) {
                auto theta = (j) * 2 * M_PI / petal_count;
                float radius = radius_augment;
                auto luon_index = luon_base_index;
                float stretch = 0;
                while (luon_index < LUON_COUNT) {
                    radius += (*harmony->luons)[luon_index - 1]->smooth_log;
                    stretch += (*harmony->luons)[luon_index - 1]->energy;
                    luon_index += luon_base_index;
                }
                if ((*harmony->luons)[luon_base_index - 1]->energy > 0.9) {
                    polygons->push_back(mv(create_petal(center_screen, radius, theta, stretch)));
                }
                luon_base_index++;
            }
            radius_augment += OBSERVATION_HEIGHT / layers / 9;
        }
        return mkuptr<Mesh>(mv(polygons));
    }

    static sptr<Polygon> create_petal(Point origin, float radius, float theta, float stretch) {
        uptr<vect<sptr<Edge>>> edges = mkuptr<vect<sptr<Edge>>>();
        auto root_vertex = mksptr<Vertex>(origin);
        edges->push_back(mv(mksptr<Edge>(
            root_vertex,
            mksptr<Vertex>(Point::from_polar(root_vertex->origin, radius, theta))
        )));
        edges->push_back(mv(mksptr<Edge>(
            root_vertex,
            mksptr<Vertex>(Point::from_polar(root_vertex->origin, radius, theta + M_PI / 2))
        )));
        edges->push_back(mv(mksptr<Edge>(
            (*edges)[0]->v2,
            mksptr<Vertex>(Point::from_polar(root_vertex->origin, radius * 3, theta + M_PI / 4))
        )));
        edges->push_back(mv(mksptr<Edge>(
            (*edges)[1]->v2,
            mksptr<Vertex>(Point::from_polar(root_vertex->origin, radius * 3, theta + M_PI / 4))
        )));
        auto polygon = mksptr<Polygon>(mv(edges));
        return polygon;
    }


    uptr<Lattice> experience() override {
        HSLColor background = HSLColor{0, 255, 255};
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background.convert_to_rgb()}, true);
        auto mesh = draw_flower();
        HSLColor base_color{128, 255, 33};
        for (auto &polygon: *mesh->polygons) {
            base_color.hue += 3;
            sketch_polygon(*polygon, lattice, base_color);
        }
        return lattice;
    }

    static void sketch_polygon(Polygon &polygon, uptr<Lattice> &lattice, HSLColor color) {
        for (auto &edge: *polygon.edges) {
            sketch_line(edge->to_line(), lattice, color);
        }
    }

    static void sketch_line(Line line, uptr<Lattice> &lattice, HSLColor color) {
        int steps = std::sqrt(std::pow(line.p2.x - line.p1.x, 2) + std::pow(line.p2.y - line.p1.y, 2));
        for (int i = 0; i <= steps; i++) {
            float ratio = scflt(i) / scflt(steps);
            int x = scint((1 - ratio) * line.p1.x + ratio * line.p2.x);
            int y = scint((1 - ratio) * line.p1.y + ratio * line.p2.y);
            lattice->set_pith(x, y, Pith{color.convert_to_rgb(), 2});
        }
    }
};
}
