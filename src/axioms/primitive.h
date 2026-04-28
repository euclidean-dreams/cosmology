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
        x{x}, y{y} {
    }

    Point(int x, int y) :
        x{scflt(x)}, y{scflt(y)} {
    }

    static Point from_polar(Point origin, float radius, float theta);
};

class Line {
public:
    Point p1;
    Point p2;

    Line(Point p1, Point p2) :
        p1{p1}, p2{p2} {
    }

    static Line from_shift(Line line, float distance);
};

class Quadrangle {
public:
    Line l1;
    Line l2;
    Line l3;
    Line l4;

    static Quadrangle from_extrusion(Line line, float distance);
};

class Vertex {
public:
    Point origin;

    explicit Vertex(Point origin) :
        origin{origin} {
    }
};

class Edge {
public:
    sptr<Vertex> v1;
    sptr<Vertex> v2;

    Edge(sptr<Vertex> v1, sptr<Vertex> v2) :
        v1{mv(v1)}, v2{mv(v2)} {
    }

    Line to_line() {
        return Line{v1->origin, v2->origin};
    }
};

class Polygon {
public:
    uptr<vect<sptr<Edge>>> edges;

    explicit Polygon(uptr<vect<sptr<Edge>>> edges) : edges{mv(edges)} {
    }
};


class Mesh {
public:
    uptr<vect<sptr<Polygon>>> polygons;

    explicit Mesh(uptr<vect<sptr<Polygon>>> polygons) : polygons{mv(polygons)} {
    }
};

template <class T>
class Signal : public Name {
private:
    vect<T> samples{};

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

    typename vect<T>::iterator begin() {
        return samples.begin();
    }

    typename vect<T>::iterator end() {
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

class Pith {
public:
    Color color;
    float magnitude;
    float twist;
    int tesselation;

    Pith(Color color) : color{color}, magnitude{1}, twist{0}, tesselation{0} {
    }

    Pith(Color color, float magnitude) : color{color}, magnitude{magnitude}, twist{0}, tesselation{0} {
    }

    Pith(Color color, float magnitude, float twist) : color{color}, magnitude{magnitude}, twist{twist},
                                                      tesselation{0} {
    }

    Pith(Color color, float magnitude, float twist, int tesselation) : color{color}, magnitude{magnitude}, twist{twist},
                                                                       tesselation{tesselation} {
    }
};

class Lattice : public Name {
private:
    umap<Coordinate, Pith, CoordinateHash> piths;

public:
    int width;
    int height;
    Pith null_pith;
    bool tessellate;

    explicit Lattice(int width, int height, Pith null_pith, bool tessellate = false);

    Pith get_pith(int x, int y) const;

    void set_pith(int x, int y, Pith pith);

    int size() const;

    bool is_valid(int x, int y) const;

    void meld(Lattice &other);

    umap<Coordinate, Pith, CoordinateHash>::iterator begin() {
        return piths.begin();
    }

    umap<Coordinate, Pith, CoordinateHash>::iterator end() {
        return piths.end();
    }
};

#ifdef threads_found

template <class T>
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
        arbit = mv(new_arbit);
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

#endif
}
