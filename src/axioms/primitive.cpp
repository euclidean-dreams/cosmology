#include "primitive.h"
#include "mathematics.h"

namespace cosmology {

HSLColor::HSLColor(int hue, int saturation, int lightness)
        : lightness{embind(0, lightness, 100)},
          saturation{embind(0, saturation, 100)} {
    while (hue < 0) {
        hue += HSL_HUE_MAX;
    }
    this->hue = hue % HSL_HUE_MAX;
}

Color HSLColor::convert_to_rgb() const {
    auto working_hue = scflt(hue);
    auto working_saturation = scflt(saturation) / 100;
    auto working_lightness = scflt(lightness) / 100;
    auto h_dabba = working_hue / 60.0f;
    auto chroma = (1.0f - std::abs(2.0f * working_lightness - 1.0f)) * working_saturation;
    auto intermediary = chroma * (1.0f - std::abs(std::fmod(h_dabba, 2.0f) - 1.0f));
    auto lightness_adjustment = working_lightness - chroma / 2.0f;
    float initial_red, initial_green, initial_blue;
    if (0.0f <= h_dabba && h_dabba <= 1.0f) {
        initial_red = chroma;
        initial_green = intermediary;
        initial_blue = 0.0f;
    } else if (1.0f <= h_dabba && h_dabba <= 2.0f) {
        initial_red = intermediary;
        initial_green = chroma;
        initial_blue = 0.0f;
    } else if (2.0f <= h_dabba && h_dabba <= 3.0f) {
        initial_red = 0.0f;
        initial_green = chroma;
        initial_blue = intermediary;
    } else if (3.0f <= h_dabba && h_dabba <= 4.0f) {
        initial_red = 0.0f;
        initial_green = intermediary;
        initial_blue = chroma;
    } else if (4.0f <= h_dabba && h_dabba <= 5.0f) {
        initial_red = intermediary;
        initial_green = 0.0f;
        initial_blue = chroma;
    } else {
        initial_red = chroma;
        initial_green = 0.0f;
        initial_blue = intermediary;
    }
    auto red = scast<uint8_t>(std::roundf((initial_red + lightness_adjustment) * 255));
    auto green = scast<uint8_t>(std::roundf((initial_green + lightness_adjustment) * 255));
    auto blue = scast<uint8_t>(std::roundf((initial_blue + lightness_adjustment) * 255));
    return Color{red, green, blue};
}

Point Point::from_polar(Point origin, float radius, float theta) {
    float x = origin.x + radius * std::cos(theta);
    float y = origin.y + radius * std::sin(theta);
    return Point{x, y};
}

Lattice::Lattice(int width, int height, Color null_color) :
        dots{},
        width{width},
        height{height},
        null_color{null_color} {

}

Color Lattice::get_color(int x, int y) const {
    auto dot = dots.find({x, y});
    if (dot != dots.end()) {
        return dot->second;
    } else {
        return null_color;
    }
}

void Lattice::set_color(int x, int y, Color color) {
    if (is_valid(x, y)) {
        dots.insert_or_assign({x, y}, color);
    }
}

int Lattice::size() const {
    return width * height;
}

bool Lattice::is_valid(int x, int y) const {
    if (x >= 0 && x < width) {
        if (y >= 0 && y < height) {
            return true;
        }
    }
    return false;
}

void Lattice::meld(Lattice &other) {
    for (auto &dot: other.dots) {
        if (dots.find(dot.first) == dots.end()) {
            dots.insert_or_assign(dot.first, dot.second);
        }
    }
}

void Lattice::paint_line(Point p1, Point p2, Color color) {
    auto m = (p2.y - p1.y) / (p2.x - p1.x);
    auto b = p1.y - m * p1.x;
    auto max_samples = std::abs(p2.x - p1.x);
    auto distance_per_sample = (p2.x - p1.x) / max_samples;
    for (int sample = 0; sample < max_samples; sample++) {
        auto x = p1.x + sample * distance_per_sample;
        auto y = m * x + b;
        set_color(x, y, color);
    }
}

void Lattice::paint_circle(Point locus, int radius, Color color) {
    auto min_x = locus.x - radius;
    if (min_x < 0) {
        min_x = 0;
    }
    auto min_y = locus.y - radius;
    if (min_y < 0) {
        min_y = 0;
    }
    auto max_x = locus.x + radius;
    if (max_x >= OBSERVATION_WIDTH) {
        max_x = OBSERVATION_WIDTH - 1;
    }
    auto max_y = locus.y + radius;
    if (max_y >= OBSERVATION_HEIGHT) {
        max_y = OBSERVATION_HEIGHT - 1;
    }
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            if (radius * radius > (x - locus.x) * (x - locus.x) + (y - locus.y) * (y - locus.y)) {
                set_color(x, y, color);
            }
        }
    }
}


Antechamber::Antechamber(int desired_loungers) :
        mutex{},
        notifier{},
        desired_loungers{desired_loungers},
        current_loungers{0} {

}

void Antechamber::lounge() {
    std::unique_lock<std::mutex> lock{mutex};
    current_loungers++;
    if (current_loungers == desired_loungers) {
        notifier.notify_all();
    } else {
        notifier.wait(lock);
    }
}

void Antechamber::clean() {
    current_loungers = 0;
}


}
