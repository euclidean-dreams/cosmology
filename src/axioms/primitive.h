#pragma once

#include <mutex>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <condition_variable>
#include "interfaces.h"
#include "constants.h"
#include "macros.h"


namespace cosmology {

struct Color {
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

constexpr int HSL_HUE_MAX = 360;

class HSLColor {
public:
    int hue;
    int saturation;
    int lightness;

    HSLColor(int hue, int saturation, int lightness);

    Color convert_to_rgb() const;
};

class Point {
public:
    float x;
    float y;

    Point(float x, float y) :
            x{x}, y{y} {}

    static Point from_polar(Point origin, float radius, float theta);
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

struct Coordinate {
    int x;
    int y;

    bool operator==(const Coordinate &other) const {
        return x == other.x && y == other.y;
    }
};

struct CoordinateHash {
    size_t operator()(const Coordinate &locus) const {
        return locus.y * OBSERVATION_WIDTH + locus.x;
    }
};

class Lattice : public Name {
private:
    umap<Coordinate, Color, CoordinateHash> dots;

public:
    int width;
    int height;
    Color null_color;

    explicit Lattice(int width, int height, Color null_color);

    Color get_color(int x, int y) const;

    void set_color(int x, int y, Color color);

    int size() const;

    bool is_valid(int x, int y) const;

    void meld(Lattice &other);

    void paint_line(Point p1, Point p2, Color color);

    umap<Coordinate, Color, CoordinateHash>::iterator begin() {
        return dots.begin();
    }

    umap<Coordinate, Color, CoordinateHash>::iterator end() {
        return dots.end();
    }
};

template<class T>
class Arbiter : public Name {
private:
    std::mutex mutex;
    sptr<const T> arbit;
    bool new_data;

public:
    Arbiter() : mutex{},
                arbit{nullptr},
                new_data{false} {

    }

    void give(uptr<const T> new_arbit) {
        std::unique_lock<std::mutex> lock{mutex};
        arbit = move(new_arbit);
        new_data = true;
    }

    sptr<const T> take() {
        std::unique_lock<std::mutex> lock{mutex};
        new_data = false;
        return arbit;
    }

    bool ready() const {
        return new_data;
    }
};

class Antechamber {
public:
    std::mutex mutex;
    std::condition_variable notifier;
    int desired_loungers;
    int current_loungers;

    Antechamber(int desired_loungers);

    void lounge();

    void clean();
};
}
