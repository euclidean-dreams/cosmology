#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;
namespace workshop {

class Spectrogram : public Name {
private:
    void record_luon_state() {
        auto new_signal = mkuptr<vect<Color >>();
        for (auto &luon: *harmony->luons) {
            auto rectified = luon->delta;
            if (rectified < 0) {
                rectified = 0;
            }
            auto anti_rectified = luon->delta;
            if (anti_rectified > 0) {
                anti_rectified = 0;
            }
            float index_scaler = TWIST * ((100 + luon->index) / 100);
            uint8_t red = embind(0, 33 * luon->energy * index_scaler, 255);
            uint8_t green = embind(0, 99 * luon->energy, 255);
            uint8_t blue = embind(0, CHAOS * -anti_rectified, 255);
            Color color = {red, green, blue};
            new_signal->push_back(color);
        }

        int index = 0;
        while (index < new_signal->size()) {
            index += RESONANCE;
            auto current_color = (*new_signal)[index];
            (*new_signal)[index] = Color{
                    scast<uint8_t>(embind(0, current_color.red + 33, 255)),
                    scast<uint8_t>(embind(0, current_color.green + 33, 255)),
                    scast<uint8_t>(embind(0, current_color.blue + 33, 255))
            };
        }

        history.push_front(mv(new_signal));
        while (history.size() > width) {
            history.pop_back();
        }
    }

public:
    int height;
    int width;
    uptr<Harmony> harmony;
    lst<uptr<vect<Color>>> history;

    Spectrogram(Psyche &psyche) : history{} {
        vect<int> luon_indices{};
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = psyche.create_harmony(luon_indices);
        width = OBSERVATION_WIDTH;
        height = OBSERVATION_HEIGHT;
        for (int i = 0; i < width; i++) {
            auto empty_signal = mkuptr<vect<Color >>();
            empty_signal->resize(LUON_COUNT, Color{0, 0, 0});
            history.push_front(mv(empty_signal));
        }
    }

    void display(Lattice &lattice) {
        record_luon_state();
        auto x = 0;
        for (auto &signal: history) {
            for (int horizontal_spread = 0; horizontal_spread < MAGNITUDE; horizontal_spread++) {
                auto y = 0;
                for (auto &color: *signal) {
                    for (int vertical_spread = 0; vertical_spread < MAGNITUDE; vertical_spread++) {
                        lattice.set_pith(x, y, Pith{color});
                        y++;
                    }
                    if (y > OBSERVATION_HEIGHT) {
                        break;
                    }
                }
                x++;
                if (x > OBSERVATION_WIDTH) {
                    break;
                }
            }
        }
    }
};


class Workshop : public Impression {
private:
    Spectrogram spectrogram;

public:
    Workshop(Psyche &psyche)
            : spectrogram(psyche) {

    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}});
        spectrogram.display(*lattice);
        return lattice;
    }
};

}
