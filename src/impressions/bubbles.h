#pragma once

#include "axioms.h"
#include "impression.h"


namespace cosmology {

class Bubble;

class Effervescence;

class Glimmer : public Name {
private:
    Bubble &bubble;
    Luon &luon;


public:
    Glimmer(Bubble &bubble, Luon &luon);

    void paint(Lattice &lattice);
};


class Bubble : public Name {
private:
    uptr<Harmony> harmony;
    vec<uptr<Glimmer>> glimmers;

public:
    Effervescence &effervescence;
    Point origin;
    float radius;
    float direction;
    Color color;
    float previous_magnitude;

    Bubble(Effervescence &effervescence, uptr<Harmony> harmony, Point origin, Color color);

    void paint(Lattice &lattice);

    void move();
};


class Effervescence : public Impression {
private:
    Psyche &psyche;
    vec<uptr<Bubble>> bubbles;

public:
    Color background_color;

    Effervescence(Psyche &psyche);

    uptr<Lattice> experience() override;
};


}
