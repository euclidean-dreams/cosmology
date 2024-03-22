#pragma once

#include "impression.h"
#include "axioms.h"

namespace cosmology {

int BASE_HUE;

class Kenning : public Name {
public:
    Luon &luon;
    Point origin;
    lst<Point> form;
    HSLColor color;


    Kenning(Luon &luon, Point origin, HSLColor color)
            : luon{luon},
              origin{origin},
              form{},
              color(color) {

    }

    void paint(Lattice &lattice) {
        color = {
                scint(BASE_HUE + luon.log_energy * 33),
                scint(75 + luon.delta),
                scint(luon.log_energy * 10)

        };
        if (luon.log_delta > 0.1) {
            Point most_recent_point = origin;
            if (not form.empty()) {
                most_recent_point = form.front();
            }
            auto new_point = Point::from_polar(most_recent_point, luon.log_delta / 3, luon.delta);
            form.push_front(new_point);
        } else {
            if (not form.empty()) {
                form.pop_back();
            }
        }
        auto rgb = color.convert_to_rgb();
        for (auto &point: form) {
            lattice.set_color(origin.x + point.x, origin.y + point.y, rgb);
        }
    }


    void move() {

    }
};

class Bard : public Circlet {
private:
    Antechamber &kickoff_antechamber;
    Antechamber &completion_antechamber;
    vec<up<Kenning>> kennings;
    int bard_index;
    vec<up<Lattice>> &lattices;


public:
    Bard(Antechamber &kickoff_antechamber,
         Antechamber &completion_antechamber,
         vec<up<Kenning>> kennings,
         int bard_index,
         vec<up<Lattice>> &lattices)
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

class Tyr : public Impression {
private:
    Psyche &psyche;
    Antechamber kickoff_antechamber;
    Antechamber completion_antechamber;
    int bard_count;
    vec<up<std::thread>> bard_threads;
    vec<up<Lattice>> lattices;

public:
    Tyr(Psyche &psyche)
            : psyche{psyche},
              bard_count{THREAD_COUNT},
              kickoff_antechamber{THREAD_COUNT + 1},
              completion_antechamber{THREAD_COUNT + 1} {
        for (int bard_index = 0; bard_index < bard_count; bard_index++) {
            vec<int> luon_indices{};
            for (int i = 0; i < LUON_COUNT; i++) {
                if (i % bard_count == bard_index) {
                    luon_indices.push_back(i);
                }
            }
            auto harmony = this->psyche.create_harmony(luon_indices);
            vec<up<Kenning>> kennings;
            for (auto &luon: *harmony->luons) {
                float x = Randomizer::generate(OBSERVATION_WIDTH);
                float y = Randomizer::generate(OBSERVATION_HEIGHT);
                int hue = scflt(luon->index) / scflt(LUON_COUNT) * (scflt(HSL_HUE_MAX) / 2);
                HSLColor color{hue, 100, 50};
                auto kenning = mkup<Kenning>(*luon, Point{x, y}, color);
                kennings.push_back(mv(kenning));
            }
            auto bard = mkup<Bard>(kickoff_antechamber, completion_antechamber, mv(kennings), bard_index, lattices);
            bard_threads.push_back(Circlet::begin(mv(bard)));
        }
    }

    up<Lattice> experience() override {
        BASE_HUE = get_current_time() / 33333 % HSL_HUE_MAX;
        auto result_lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (int i = 0; i < bard_count; i++) {
            auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
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
