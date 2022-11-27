#pragma once

#include <stdio.h>

#include "ScaleHandler.hpp"

#include "Otter.hpp"


class QueueState
{
public:
    
    void setQueue(const otter::Queue& queue);
    otter::Queue queue;
    otter::Rectangle queueBounds;
    otter::Rectangle displayedBounds;
    
    void recomputeScale();
    ScaleHandler scaleHandler;
    otter::Vec2 windowTopLeft_screenSpace;
    otter::Vec2 windowSize_screenSpace;
    
    void SetMousePos(const otter::Vec2& mousePos_screenSpace);
    otter::Vec2 mousePos_otterSpace = otter::Vec2(0, 0);
    
    void SetMouseClick(const otter::Vec2& mouseClick_screenSpace);
    otter::Vec2 mouseClick_otterSpace = otter::Vec2(0, 0);
};