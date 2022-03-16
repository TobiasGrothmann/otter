#pragma once
#include "core.hpp"

struct State
{
    State() = default;
    State(std::shared_ptr<otter::Queue> queue)
        : queue(queue)
    {
        updateStats();
    }
    
    bool isValid() const
    {
        return (bool)queue;
    }
    
    void updateStats()
    {
        // travel and paint
        std::vector<double> paintAndTravelDist = queue->getPaintAndTravelDistance();
        paintDist = paintAndTravelDist[0];
        travelDist = paintAndTravelDist[1];
        
        // bounds
        queueBounds = queue->getBounds();
        queueBounds.bottomLeft = otter::Vec2(std::min(queueBounds.bottomLeft.x, 0.0), std::min(queueBounds.bottomLeft.y, 0.0));
        const otter::Vec2 margin = queueBounds.getSize() * queueMarginFactor;
        queueBounds.bottomLeft -= margin;
        queueBounds.topRight += margin;
        queueSize = queueBounds.getSize();
        
        // quadtree
        quadTree = otter::QuadTree(*queue, 8, true, otter::SampleSettings(8.0));
    }
    
    std::shared_ptr<otter::Queue> queue = {};
    
    // SETTINGS
    double queueMarginFactor = 0.035;
    
    // STATS
    double paintDist = {};
    double travelDist = {};
    
    otter::Vec2 queueSize = {};
    otter::Rectangle queueBounds = {};
    
    otter::QuadTree quadTree = otter::QuadTree();
    
    otter::PlottablePtr currentlyHoveredPlottable = nullptr;
};
