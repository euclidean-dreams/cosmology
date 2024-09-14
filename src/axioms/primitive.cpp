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

Lattice::Lattice(int width, int height, Pith null_pith, bool tessellate) :
        piths{},
        width{width},
        height{height},
        null_pith{null_pith},
        tessellate{tessellate} {

}

Pith Lattice::get_pith(int x, int y) const {
    auto dot = piths.find({x, y});
    if (dot != piths.end()) {
        return dot->second;
    } else {
        return null_pith;
    }
}

void Lattice::set_pith(int x, int y, Pith pith) {
    if (is_valid(x, y)) {
        piths.insert_or_assign({x, y}, pith);
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
    for (auto &dot: other.piths) {
        if (piths.find(dot.first) == piths.end()) {
            piths.insert_or_assign(dot.first, dot.second);
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
