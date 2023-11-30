#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class Glimmer : public Name {
public:
    Luon &luon;
    Point origin;

    Glimmer(Luon &luon, Point origin) : luon{luon}, origin{origin} {

    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 4444 * luon.log_energy * DIRECTION;
        float direction = 333 * luon.fundamental * (2 * M_PI) / scflt(LUON_COUNT);
        origin = Point::from_polar(origin, distance, direction);

        // gravity
        origin = Point::from_polar(origin, luon.fundamental / 36963, -direction);

        if (origin.x < 0 || origin.x > OBSERVATION_WIDTH) {
            origin.x = scflt(OBSERVATION_WIDTH) / 2;
            origin.y = scflt(OBSERVATION_HEIGHT) / 2;
        }
        if (origin.y < 0 || origin.y > OBSERVATION_HEIGHT) {
            origin.x = scflt(OBSERVATION_WIDTH) / 2;
            origin.y = scflt(OBSERVATION_HEIGHT) / 2;
        }
    }
};

class Glimmering : public Name {
public:
    vec<up<Glimmer>> glimmers;

    Glimmering() : glimmers{} {};

    void create_glimmer(Luon &luon, Point origin) {
        auto glimmer = mkup<Glimmer>(luon, origin);
        glimmers.push_back(mv(glimmer));
    }
};

class Square {
public:
    Glimmer &glimmer;

    Square(Glimmer &glimmer) : glimmer{glimmer} {

    }

    void paint(Lattice &lattice) {
        auto size = scflt(OBSERVATION_WIDTH) / 333 * glimmer.luon.log_energy * MAGNITUDE;
        for (int y = glimmer.origin.y - size; y < glimmer.origin.y + size; y++) {
            for (int x = glimmer.origin.x - size; x < glimmer.origin.x + size; x++) {
                auto fade = std::abs(x - glimmer.origin.x) + std::abs(y - glimmer.origin.y);
                uint8_t red = embind(0, 99 * glimmer.luon.log_energy - fade, 255);
                uint8_t green = embind(0, 33 * glimmer.luon.log_energy - fade, 255);
                uint8_t blue = embind(0, 66 * glimmer.luon.log_energy - fade, 255);
                lattice.set_color(x, y, Color{red, green, blue});
            }
        }
    }

};

class Constellation : public Name {
public:
    lst<Glimmer *> stars{};
    Color color;
    int fundamental;
    Point fundamental_point;

    Constellation(int fundamental) :
            stars{},
            fundamental{fundamental},
            fundamental_point{
                    Point::from_polar(
                            {scflt(OBSERVATION_WIDTH / 2), scflt(OBSERVATION_HEIGHT / 2)},
                            OBSERVATION_WIDTH / 4,
                            scflt(333 * scflt(fundamental) * (2 * M_PI) / scflt(LUON_COUNT)
                            )
                    )
            } {
        uint8_t red = embind(0, 166 + 33 * Randomizer::generate_proportion(), 255);
        uint8_t green = embind(0, 33 + 33 * Randomizer::generate_proportion(), 255);
        uint8_t blue = embind(0, 199 + 33 * Randomizer::generate_proportion(), 255);
        color = {red, green, blue};
    }

    void observe(Lattice &lattice) {
        float star_sum = 0;
        for (auto star: stars) {
            star_sum += star->luon.energy;
        }
        auto mean = star_sum / stars.size();

        auto size = scflt(OBSERVATION_WIDTH) / 222 * mean * MAGNITUDE;
        auto origin = stars.front()->origin;
        for (int y = origin.y - size; y < origin.y + size; y++) {
            for (int x = origin.x - size; x < origin.x + size; x++) {
                lattice.set_color(x, y, color);
            }
        }
    }
};

class StarrySky : public Name {
private:
    Glimmering &glimmering;
    lst<up<Constellation>> constellations;

public:
    StarrySky(Glimmering &glimmering) : glimmering{glimmering} {

    };

    void constellate() {
        for (int fundamental = 2; fundamental < LUON_COUNT / 2; fundamental++) {
            auto constellation = mkup<Constellation>(fundamental);
            for (auto &glimmer: this->glimmering.glimmers) {
                if (glimmer->luon.fundamental % fundamental == 0) {
                    constellation->stars.push_front(glimmer.get());
                }
            }
            constellations.push_back(mv(constellation));
        }
    };

    void observe(Lattice &lattice) {
        for (auto &constellation: constellations) {
            constellation->observe(lattice);
        }
    }
};

class HappyPinkHearts : public Name {
private:
    Glimmering glimmering;
    up<Harmony> harmony;
    vec<up<Square>> squares;
    StarrySky starry_sky;

public:
    HappyPinkHearts(up<Harmony> harmony) :
            harmony{mv(harmony)},
            glimmering{},
            starry_sky{glimmering} {
        for (auto &luon: *this->harmony->luons) {
            float x = OBSERVATION_WIDTH / 2;
            float y = OBSERVATION_HEIGHT / 2;
            glimmering.create_glimmer(*luon, Point{x, y});
        }
        for (auto &glimmer: glimmering.glimmers) {
            auto square = mkup<Square>(*glimmer);
            squares.push_back(mv(square));
        }
        starry_sky.constellate();
    }

    up<Lattice> observe() {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &glimmer: glimmering.glimmers) {
            glimmer->move();
        }
        starry_sky.observe(*lattice);
        for (auto &square: squares) {
            square->paint(*lattice);
        }
        return lattice;
    }
};

}
