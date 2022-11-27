#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "QueueState.hpp"


class OtterSketch
{
public:
    
    otter::Queue create(const QueueState& state);
};
