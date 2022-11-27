#pragma once

#include "Otter.hpp"

#include "ofMain.h"


class ScaleHandler
{
public:
    ScaleHandler() = default;
    
    void setScale(const otter::Vec2& canvasSize, const otter::Vec2& canvasPos, const otter::Rectangle& queueBounds);
    
    void addScroll(const otter::Vec2& offset);
    void resetScroll();
    
    void addZoom(double offset);
    void resetZoom();
    
    // scale from otter space to screen space
    ofVec2f scale(const otter::Vec2& otterPoint) const;
    double scale(double distance) const;
    
    // scale from screen space to otter space
    otter::Vec2 scaleReverse(const otter::Vec2& screenPoint) const;
    double scaleReverse(double distance) const;
    
private:
    double queueScale = 0.0;
    otter::Vec2 queueOffset = {};
    otter::Vec2 canvasPos_screen = {};
    otter::Vec2 canvasSize_screen = {};
    otter::Rectangle queueBounds = {};
    
    otter::Vec2 scrolling_screen = {};
    otter::Vec2 zoomCenter_screen = {};
    double zoom = 0.0;
};
