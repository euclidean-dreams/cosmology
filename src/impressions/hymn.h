#pragma once

#include "impression.h"
#include "axioms.h"

namespace cosmology {

class Kenning : public Name {
public:
    Luon &luon;
    Point origin;
    float direction;

    Kenning(Luon &luon, Point origin)
            : luon{luon},
              origin{origin},
              direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {

    }

    void paint(Lattice &lattice) {
        auto hue = (scflt(luon.index) / scflt(LUON_COUNT) * HSL_HUE_MAX) + get_current_time() / 33333 % HSL_HUE_MAX;
        for (int i = 0; i < luon.energy / 100; i++) {
            auto color = HSLColor{scint(hue), scint(luon.energy) + i, 44}.convert_to_rgb();
            Point new_origin{origin.x + Randomizer::generate(i) * Randomizer::generate_sign(),
                             origin.y + Randomizer::generate(i) * Randomizer::generate_sign()};
            gyfu(lattice, new_origin, luon.energy * MAGNITUDE / 33, 0, color);
        }
    }

    void gyfu(Lattice &lattice, Point previous_point, float magnitude, int depth, Color color) {
        if (magnitude > 1) {
            auto direction = Randomizer::generate(4);
            Point current_point{previous_point.x, previous_point.y};
            if (direction == 0) {
                current_point.x++;
            } else if (direction == 1) {
                current_point.x--;
            } else if (direction == 2) {
                current_point.y++;
            } else if (direction == 3) {
                current_point.y--;
            }
            Color modified_color{
                    scast<uint8_t>(embind(0, color.red + Randomizer::generate(2), 255)),
                    scast<uint8_t>(embind(0, color.green + Randomizer::generate(2), 255)),
                    scast<uint8_t>(embind(0, color.blue + Randomizer::generate(2), 255)),
            };
            lattice.set_color(current_point.x, current_point.y, modified_color);
            gyfu(lattice, current_point, magnitude - 0.1, depth + 1, modified_color);
        }
    }


    void move() {
        float distance = (0.1 + abs(luon.delta / 5)) * MOVEMENT;
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
        direction += 0;
    }


};

class Hymn : public Impression {
private:
    Psyche &psyche;
    vec<up<Kenning>> kennings;

public:
    Hymn(Psyche &psyche) : psyche{psyche} {
        vec<int> luon_indices{};
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto kenning = mkup<Kenning>(*luon, Point{x, y});
            kennings.push_back(mv(kenning));
        }
    }

    up<Lattice> experience() override {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &kenning: kennings) {
            kenning->move();
            kenning->paint(*lattice);
        }
        return lattice;
    }
};

}
