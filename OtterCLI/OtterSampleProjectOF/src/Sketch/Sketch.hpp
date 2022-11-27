#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "State.hpp"


class Sketch
{
public:
    
    void setup();
    otter::Queue create(const State& state);
    
    bool updateEveryFrame = false;
    bool updateOnMouseMove = false;
    bool updateOnMouseClick = false;
};
