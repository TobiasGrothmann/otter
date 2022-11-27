#pragma once

#include <stdio.h>

#include "Otter.hpp"

#include "ScaleHandler.hpp"
#include "QueueState.hpp"

#define GRID_COL(VAL) ofColor((VAL) * 255.0f * 0.8f, (VAL) * 255.0f, (VAL) * 255.0f * 0.8f, 255.0f * 0.6f)


class QueueDrawer
{
public:
    void draw(const QueueState& queueState);
    
private:
    void drawGrid(const QueueState& queueState);
    void drawQueue(const QueueState& queueState);
    
    ofColor colorGrid = GRID_COL(1.0f - 0.185f);
    ofColor colorGridHighlight = GRID_COL(1.0f - 0.42f);
    ofColor colorGridZero = GRID_COL(1.0f - 0.9f);
};
