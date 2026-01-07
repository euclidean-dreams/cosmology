#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;

namespace curvature {
class Path : public Name {
public:
    Luon &luon;
    Luon &upper_partial;
    Point origin;
    lst<Point> loci;
    HSLColor color;
    float direction;
    float rotation;
    float damper;

    Path(Luon &luon, Luon &upper_partial, Point origin, HSLColor color)
        : luon{luon},
          upper_partial{upper_partial},
          origin{origin},
          loci{},
          color{color},
          direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)},
          rotation{0} {
        auto current_point = origin;
        for (int i = 0; i < luon.energy + 10; i++) {
            loci.push_back(current_point);
            current_point = Point::from_polar(origin, i, direction);
        }
    }

    void paint(Lattice &lattice) {
        if (luon.energy > 0.1) {
            float delta = M_PI / 2;
            float t = 0;
            while (t < 10) {
                float x = MAGNITUDE * 3 * luon.smooth_log * std::sin(luon.energy * t + delta);
                float y = MAGNITUDE * 3 * luon.smooth_log * std::sin(upper_partial.energy * t);
                loci.emplace_front(x, y);
                t += 0.1 / luon.energy;
            }
            while (loci.size() > 3333 * luon.energy) {
                loci.pop_back();
            }
        }
        for (auto &point: loci) {
            auto adjusted_point = Point{origin.x + point.x, origin.y + point.y};
            int hue = cyclic_embind(0, COLOR + luon.index + 2, HSL_HUE_MAX);
            auto brush_size = luon.smooth_log;
            for (int i = 0; i < brush_size; i++) {
                auto current_color = HSLColor{hue, color.saturation - i / 5, color.lightness};
                auto rotated_point = Point::from_polar(adjusted_point, i * 10, rotation);
                auto magnitude = luon.smooth_log * 6;
                lattice.set_pith(scint(rotated_point.x), scint(rotated_point.y),
                                 Pith{current_color.convert_to_rgb(), magnitude});
                rotation += luon.delta / 10;
            }
        }
    }

    void move() {
        float distance = luon.energy / 9 * MOVEMENT;
        origin = Point::from_polar(origin, distance, direction);
        if (origin.x < 0) {
            origin.x = 0;
            direction += M_PI;
        } else if (origin.x >= OBSERVATION_WIDTH) {
            origin.x = OBSERVATION_WIDTH - 1;
            direction += M_PI;
        }
        if (origin.y < 0) {
            origin.y = 0;
            direction += M_PI;
        } else if (origin.y >= OBSERVATION_HEIGHT) {
            origin.y = OBSERVATION_HEIGHT - 1;
            direction += M_PI;
        }
        lst<Point> new_loci{};
        auto index = 0;
        for (auto &locus: loci) {
            index++;
            auto theta = index;
            auto radius = std::exp(std::sin(theta))
                - 2 * std::cos(4 * theta)
                + std::pow(std::sin(1 / 24 * (2 * theta - M_PI)), 5);
            auto current_point = Point::from_polar(locus, radius, theta);
            if (current_point.x < 0) {
                current_point.x = origin.x;
            } else if (current_point.x >= OBSERVATION_WIDTH) {
                current_point.x = origin.x;
            }
            if (current_point.y < 0) {
                current_point.y = origin.y;
            } else if (origin.y >= OBSERVATION_HEIGHT) {
                current_point.y = origin.y;
            }
            new_loci.push_back(current_point);
        }
        loci = new_loci;
    }
};

class Curvature : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Path>> paths;

public:
    Curvature(Psyche &psyche)
        : psyche{psyche},
          paths{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        auto index = 0;
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto color = HSLColor{
                Randomizer::generate(HSL_HUE_MAX),
                50 + Randomizer::generate(50),
                33 + Randomizer::generate(50)
            };
            auto upper_partial_index = index * 2;
            if (upper_partial_index < harmony->luons->size()) {
                sptr<Luon> upper_partial = (*harmony->luons)[upper_partial_index];
                auto path = mkuptr<Path>(*luon, *upper_partial, Point{x, y}, color);
                paths.push_back(mv(path));
                index++;
            } else {
                break;
            }
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        for (auto &curve: paths) {
            curve->move();
            curve->paint(*lattice);
        }
        return lattice;
    }
};
}
