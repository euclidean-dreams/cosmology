#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

class Spectrogram : public Name {
private:
    void record_luon_state() {
        auto new_signal = mkuptr<vec<Color>>();
        for (auto &luon: *harmony.luons) {
            auto rectified = luon->delta;
            if(rectified < 0) {
                rectified = 0;
            }
            auto anti_rectified = luon->delta;
            if(anti_rectified > 0) {
                anti_rectified = 0;
            }
            uint8_t red = embind(0, 200 * rectified, 255);
            uint8_t green = embind(0, 3 * luon->energy, 255);
            uint8_t blue = embind(0, 200 * -anti_rectified, 255);
            Color color = {red, green, blue};
            new_signal->push_back(color);
        }

        int index = 0;
        while (index < new_signal->size()) {
            index += MOVEMENT * 10;
            (*new_signal)[index] = {99, 99, 99};
        }

        history.push_front(mv(new_signal));
        while (history.size() > width) {
            history.pop_back();
        }
    }

public:
    int height;
    int width;
    Harmony &harmony;
    lst<uptr<vec<Color>>> history;

    Spectrogram(Harmony &harmony) : harmony{harmony}, history{} {
        width = OBSERVATION_WIDTH;
        height = OBSERVATION_HEIGHT;
        for (int i = 0; i < width; i++) {
            auto empty_signal = mkuptr<vec<Color>>();
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
                        lattice.set_color(x, y, color);
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
    uptr<Harmony> harmony;
    Spectrogram spectrogram;

public:
    Workshop(uptr<Harmony> harmony) :
            harmony{mv(harmony)},
            spectrogram(*this->harmony) {

    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        spectrogram.display(*lattice);
        return lattice;
    }
};

}
