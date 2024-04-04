#include "bubbles.h"

namespace cosmology {


Glimmer::Glimmer(Bubble &bubble, Luon &luon) : bubble{bubble}, luon{luon} {

}

void Glimmer::paint(Lattice &lattice) {
    auto size = luon.energy / 33;
    uint8_t red = embind(0, Randomizer::generate(2 * bubble.color.red), 255);
    uint8_t green = embind(0, Randomizer::generate(2 * bubble.color.green), 255);
    uint8_t blue = embind(0, Randomizer::generate(2 * bubble.color.blue), 255);
    for (int i = 0; i < size; i++) {
        auto theta = i * ((2 * M_PI) / size) + (2 * M_PI) * Randomizer::generate_proportion();
        auto point = Point::from_polar(bubble.origin, bubble.radius * Randomizer::generate_proportion(), theta);
        lattice.set_color(point.x, point.y, Color{red, green, blue});
    }
}

Bubble::Bubble(Effervescence &effervescence, std::unique_ptr<Harmony> harmony, Point origin, Color color)
        : effervescence{effervescence},
          harmony{mv(harmony)},
          origin{origin},
          color{color},
          radius{scflt(this->harmony->luons->size())},
          direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)},
          previous_magnitude{0}{
    for (auto &luon: *this->harmony->luons) {
        auto glimmer = mkuptr<Glimmer>(*this, *luon);
        glimmers.push_back(mv(glimmer));
    }
}

void Bubble::paint(Lattice &lattice) {
    auto working_radius = embind_flt(0, radius, OBSERVATION_WIDTH / 3);
    for (int border_circles = 0; border_circles < 2; border_circles++) {
        color.red = embind(0, effervescence.background_color.red + Randomizer::generate(99) + border_circles * 33, 255);
        color.green = embind(0, effervescence.background_color.green + Randomizer::generate(99) + border_circles * 33, 255);
        color.blue = embind(0, effervescence.background_color.blue + Randomizer::generate(99) + border_circles * 33, 255);
        float tincrement = 1 / working_radius;
        for (float t = 0.0; t < 2 * M_PI; t += tincrement) {
            float x = origin.x + working_radius * std::cos(t);
            float y = origin.y + working_radius * std::sin(t);
            lattice.set_color(x, y, color);
        }
        for (auto &glimmer: glimmers) {
            glimmer->paint(lattice);
        }
        working_radius++;
    }
}

void Bubble::move() {
    float energy_sum = 0;
    for (auto &luon: *harmony->luons) {
        energy_sum += luon->energy;
    }
    auto energy_mean = energy_sum / harmony->luons->size();
    auto delta = energy_mean - previous_magnitude;
    previous_magnitude = energy_mean;

    float distance = (0.1 + delta) * MOVEMENT * 10;
    radius = energy_mean * MAGNITUDE;
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
    direction += delta * TWIST;
}

Effervescence::Effervescence(Psyche &psyche) : psyche{psyche}, background_color{101, 152, 214} {
    // split psyche into harmonies based on fundamental + upper partial combinations
    for (int fundamental = 2; fundamental < LUON_COUNT / (2 * RESONANCE); fundamental++) {
        vec<int> luon_indices{};
        for (int upper_partial_index = fundamental;
             upper_partial_index < LUON_COUNT;
             upper_partial_index += fundamental) {
            luon_indices.push_back(upper_partial_index);
        }
        float x = Randomizer::generate(OBSERVATION_WIDTH);
        float y = Randomizer::generate(OBSERVATION_HEIGHT);
        uint8_t red = embind(0, Randomizer::generate(100), 80);
        uint8_t green = embind(0, Randomizer::generate(300), 160);
        uint8_t blue = embind(30, Randomizer::generate(500), 255);
        auto bubble = mkuptr<Bubble>(*this, psyche.create_harmony(luon_indices), Point{x, y}, Color{red, green, blue});
        bubbles.push_back(mv(bubble));
    }
}

uptr<Lattice> Effervescence::experience() {
    if (Randomizer::generate(100) > 90) {
        background_color.red = embind(100, background_color.red + Randomizer::generate_sign() * CHAOS, 255);
        background_color.green = embind(100, background_color.green + Randomizer::generate_sign() * CHAOS, 255);
        background_color.blue = embind(100, background_color.blue + Randomizer::generate_sign() * CHAOS, 255);
    }
    auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, background_color);
    for (auto &bubble: bubbles) {
        bubble->move();
        bubble->paint(*lattice);
    }
    return lattice;
}
}
