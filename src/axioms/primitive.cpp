#include "primitive.h"

namespace cosmology {

Point Point::from_polar(Point origin, float radius, float theta) {
    float x = origin.x + radius * std::cosf(theta);
    float y = origin.y + radius * std::sinf(theta);
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
