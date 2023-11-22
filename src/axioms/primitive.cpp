#include "primitive.h"

namespace cosmology {

Point Point::from_polar(Point origin, float radius, float theta) {
    float x = origin.x + radius * cos(theta);
    float y = origin.y + radius * sin(theta);
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

}
