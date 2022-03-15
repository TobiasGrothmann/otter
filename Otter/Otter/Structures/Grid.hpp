#pragma once
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Rectangle.hpp"
#include "../Plottables/Plottable.hpp"

namespace otter {

class Grid
{
public:
    Grid(const Vec2& bottomLeft, const Vec2& topRight, int xs, int ys, Vec2 margin);
    Grid(const Rectangle& bounds, int xs, int ys, Vec2 margin);
    Grid(const std::shared_ptr<Rectangle>& bounds, int xs, int ys, Vec2 margin);
    
    void setUseOnlySquareCells(bool shouldBeSquare);
    void correctXSizeToBeSquare();
    
    Rectangle getCell(int x, int y) const;
    
private:
    Rectangle totalBounds;
    int xs;
    int ys;
    Vec2 margin;
    
    bool square = false;
};

}
