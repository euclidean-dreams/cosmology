#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;


namespace spherics {
class Path : public Name {
public:
    Luon &luon;

    Path(Luon &luon)
        : luon{luon} {
    }

    void paint(Point origin, Lattice &lattice, CentrifugalPalette &palette) {
        auto max_t = scint(luon.energy) * 99;
        for (auto t = 0; t < max_t; t++) {
            auto rho = 33 + luon.smooth_log * 9 * MAGNITUDE;
            auto theta = luon.energy / 33 * scflt(t) * ((2 * M_PI) / max_t) * TWIST;
            auto phi = luon.delta / 3 * scflt(t) * ((2 * M_PI) / max_t) * RESONANCE;
            auto x = rho * std::sin(theta) * std::cos(phi);
            auto y = rho * std::sin(theta) * std::sin(phi);
            auto z = rho * cos(theta);
            auto magnitude = scint(luon.energy);
            auto palette_color = palette.get_color();
            auto color = HSLColor(palette_color.hue, embind(25, magnitude, 100), embind(0, magnitude, 100));
            auto pith = Pith{color.convert_to_rgb(), 3};
            auto screen_x = scint(x) + scint(origin.x);
            auto screen_y = scint(z) + scint(origin.y);
            lattice.set_pith(screen_x, screen_y, pith);
        }
    }
};


class Sphere : public Name {
private:
    uptr<Harmony> harmony;
    Point origin;
    float direction;
    vect<uptr<Path>> paths;
    uptr<CentrifugalPalette> palette;

public:
    Sphere(Psyche &psyche, Luon &root_luon, Point origin)
        : origin{origin},
          direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {
        vect<int> luon_indices{};
        if (root_luon.index == 0) {
            luon_indices.push_back(0);
        } else {
            for (int i = root_luon.index; i < psyche.size(); i = i + root_luon.index) {
                luon_indices.push_back(i);
            }
        }
        harmony = psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            auto path = mkuptr<Path>(*luon);
            paths.push_back(mv(path));
        }

        auto palette_harmony = psyche.create_harmony(luon_indices);
        palette = mkuptr<CentrifugalPalette>(mv(palette_harmony), 33);
    }

    void show(Lattice &lattice) {
        palette->step();
        auto path_index = 0;
        for (auto &path: paths) {
            if (path_index < scint(8 * CHAOS)) {
                path->paint(origin, lattice, *palette);
            }
            path_index++;
        }
    }

    void move() {
        float distance = std::abs(harmony->energy() / 33) * MOVEMENT;
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

class Spherics : public Impression {
private:
    Psyche &psyche;
    uptr<Harmony> harmony;
    vect<uptr<Sphere>> spheres;

public:
    Spherics(Psyche &psyche)
        : psyche{psyche} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            int x = Randomizer::generate(OBSERVATION_WIDTH);
            int y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto sphere = mkuptr<Sphere>(psyche, *luon, Point{x, y});
            spheres.push_back(mv(sphere));
        }
    }


    uptr<Lattice> experience() override {
        auto background = HSLColor{0, 0, 0};
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background.convert_to_rgb()}, true);
        for (auto &sphere: spheres) {
            sphere->move();
            sphere->show(*lattice);
        }
        return lattice;
    }
};
}
