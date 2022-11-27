#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "QueueState.hpp"


class Sketch
{
public:
    
    void setup();
    otter::Queue create(const QueueState& state);
    
    bool updateEveryFrame = false;
    bool updateOnMouseMove = false;
    bool updateOnMouseClick = false;
};
