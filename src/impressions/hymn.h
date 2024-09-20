#pragma once

#include "impression.h"
#include "axioms.h"

using namespace cosmology;
namespace hymn {

class Kenning : public Name {
public:
    Luon &luon;
    Point origin;
    float move_direction;

    Kenning(Luon &luon, Point origin)
            : luon{luon},
              origin{origin},
              move_direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {

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
            auto spread_direction = Randomizer::generate(4);
            Point current_point{previous_point.x, previous_point.y};
            if (spread_direction == 0) {
                current_point.x++;
            } else if (spread_direction == 1) {
                current_point.x--;
            } else if (spread_direction == 2) {
                current_point.y++;
            } else if (spread_direction == 3) {
                current_point.y--;
            }
            Color modified_color{
                    scast<uint8_t>(embind(0, color.red + Randomizer::generate(2), 255)),
                    scast<uint8_t>(embind(0, color.green + Randomizer::generate(2), 255)),
                    scast<uint8_t>(embind(0, color.blue + Randomizer::generate(2), 255)),
            };
            lattice.set_pith(current_point.x, current_point.y, Pith{modified_color});
            gyfu(lattice, current_point, magnitude - 0.1, depth + 1, modified_color);
        }
    }


    void move() {
        float distance = (0.1 + abs(luon.delta / 5)) * MOVEMENT;
        origin = Point::from_polar(origin, distance, move_direction);

        if (origin.x < 0) {
            origin.x = 0;
            move_direction += M_PI;
        } else if (origin.x >= OBSERVATION_WIDTH) {
            origin.x = OBSERVATION_WIDTH - 1;
            move_direction += M_PI;
        }
        if (origin.y < 0) {
            origin.y = 0;
            move_direction += M_PI;
        } else if (origin.y >= OBSERVATION_HEIGHT) {
            origin.y = OBSERVATION_HEIGHT - 1;
            move_direction += M_PI;
        }
        move_direction += 0;
    }
};

class Bard : public Circlet {
private:
    Antechamber &kickoff_antechamber;
    Antechamber &completion_antechamber;
    vect<uptr<Kenning>> kennings;
    int bard_index;
    vect<uptr<Lattice>> &lattices;


public:
    Bard(Antechamber &kickoff_antechamber,
         Antechamber &completion_antechamber,
         vect<uptr<Kenning>> kennings,
         int bard_index,
         vect<uptr<Lattice>> &lattices)
            : kickoff_antechamber{kickoff_antechamber},
              completion_antechamber{completion_antechamber},
              kennings{mv(kennings)},
              bard_index{bard_index},
              lattices{lattices} {

    }

    void activate() override {
        kickoff_antechamber.lounge();
        Deadline deadline{RENDER_TIMEOUT_MICROSECONDS};
        for (auto &kenning: kennings) {
            kenning->move();
            kenning->paint(*lattices[bard_index]);
            if (deadline.finished()) {
                break;
            }
        }
        completion_antechamber.lounge();
    }

    uint64_t get_tick_interval() override {
        return 0;
    }
};

class Hymn : public Impression {
private:
    Psyche &psyche;
    Antechamber kickoff_antechamber;
    Antechamber completion_antechamber;
    int bard_count;
    vect<uptr<std::thread>> bard_threads;
    vect<uptr<Lattice>> lattices;

public:
    Hymn(Psyche &psyche)
            : psyche{psyche},
              bard_count{THREAD_COUNT},
              kickoff_antechamber{THREAD_COUNT + 1},
              completion_antechamber{THREAD_COUNT + 1} {
        for (int bard_index = 0; bard_index < bard_count; bard_index++) {
            vect<int> luon_indices{};
            for (int i = 0; i < LUON_COUNT; i++) {
                if (i % bard_count == bard_index) {
                    luon_indices.push_back(i);
                }
            }
            auto harmony = this->psyche.create_harmony(luon_indices);
            vect<uptr<Kenning>> kennings;
            for (auto &luon: *harmony->luons) {
                float x = Randomizer::generate(OBSERVATION_WIDTH);
                float y = Randomizer::generate(OBSERVATION_HEIGHT);
                auto kenning = mkuptr<Kenning>(*luon, Point{x, y});
                kennings.push_back(mv(kenning));
            }
            auto bard = mkuptr<Bard>(kickoff_antechamber, completion_antechamber, mv(kennings), bard_index, lattices);
            bard_threads.push_back(Circlet::begin(mv(bard)));
        }
    }

    uptr<Lattice> experience() override {
        auto result_lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}});
        for (int i = 0; i < bard_count; i++) {
            auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}});
            lattices.push_back(mv(lattice));
        }

        kickoff_antechamber.lounge();
        kickoff_antechamber.clean();
        completion_antechamber.lounge();
        completion_antechamber.clean();

        for (auto &lattice: lattices) {
            result_lattice->meld(*lattice);
        }
        lattices.clear();
        return result_lattice;
    }
};

}
