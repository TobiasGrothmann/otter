#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "ScaleHandler.hpp"
#include "State.hpp"

#define GRID_COL(VAL) ofColor((VAL) * 255.0f * 0.8f, (VAL) * 255.0f, (VAL) * 255.0f * 0.8f, 255.0f * 0.6f)


class QueueDrawer
{
public:
    void draw(const State& queueState);
    
private:
    void drawGrid(const State& queueState);
    void drawQueue(const State& queueState);
    
    ofColor colorGrid = GRID_COL(1.0f - 0.185f);
    ofColor colorGridHighlight = GRID_COL(1.0f - 0.42f);
    ofColor colorGridZero = GRID_COL(1.0f - 0.9f);
};
