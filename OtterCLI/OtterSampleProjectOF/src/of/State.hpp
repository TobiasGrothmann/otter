#pragma once

#include <stdio.h>

#include "ScaleHandler.hpp"

#include "Otter.hpp"


class State
{
public:
    
    void setQueue(const otter::Queue& queue);
    otter::Queue queue;
    otter::Rectangle queueBounds;
    otter::Rectangle displayedBounds;
    int iteration = 0;
    
    void recomputeScale();
    ScaleHandler scaleHandler;
    otter::Vec2 windowTopLeft_screenSpace;
    otter::Vec2 windowSize_screenSpace;
    
    void SetMousePos(const otter::Vec2& mousePos_screenSpace);
    otter::Vec2 mousePos = otter::Vec2(0, 0);
    
    void SetMouseClick(const otter::Vec2& mouseClick_screenSpace);
    otter::Vec2 mouseClick = otter::Vec2(0, 0);
    
    void update();
    double time = 0.0;
    
    bool controlDown = false;
    bool altDown = false;
    bool commandDown = false;
    bool shiftDown = false;
};
