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
    up<Harmony> harmony;
    vec<up<Glimmer>> glimmers;

public:
    Effervescence &effervescence;
    Point origin;
    float radius;
    float direction;
    Color color;
    float previous_magnitude;

    Bubble(Effervescence &effervescence, up<Harmony> harmony, Point origin, Color color);

    void paint(Lattice &lattice);

    void move();
};


class Effervescence : public Impression {
private:
    Psyche &psyche;
    vec<up<Bubble>> bubbles;

public:
    Color background_color;

    Effervescence(Psyche &psyche);

    up<Lattice> experience() override;
};


}
