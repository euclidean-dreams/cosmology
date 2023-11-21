#pragma once

#include <cmath>
#include "interfaces.h"

namespace cosmology {

struct Color {
public:
    int red;
    int green;
    int blue;
    int alpha;
};


class Point {
public:
    float x;
    float y;
    float z;

    Point(float x, float y) :
            x{x}, y{y}, z{0} {}

    static Point from_polar(Point origin, float radius, float theta) {
        return Point{origin.x + radius * cos(theta), origin.y + radius * sin(theta)};
    }
};


template<class T>
class Signal : public Name {
private:
    vec<T> samples{};

public:
    void push_back(T sample) {
        samples.push_back(sample);
    }

    T get_sample(int index) {
        return samples[index];
    }

    void set_sample(int index, T sample) {
        samples[index] = sample;
    }

    typename vec<T>::iterator begin() {
        return samples.begin();
    }

    typename vec<T>::iterator end() {
        return samples.end();
    }

    T *get_data_pointer() {
        return samples.data();
    }

    size_t size() {
        return samples.size();
    }
};

}
