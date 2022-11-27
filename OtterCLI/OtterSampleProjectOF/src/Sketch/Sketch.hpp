#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "QueueState.hpp"


class Sketch
{
public:
    
    otter::Queue create(const QueueState& state);
};
