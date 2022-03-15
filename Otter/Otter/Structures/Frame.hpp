#pragma once
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Plottables/Rectangle.hpp"

namespace otter {

class Frame {
public:
    Frame(const Vec2& bottomLeft, const Vec2& size, const Vec2& margin);
    Frame(const Vec2& bottomLeft, const Vec2& size, double margin);
    Frame(const Vec2& size, const Vec2& margin);
    Frame(const Vec2& size, double margin);
    
    std::shared_ptr<Rectangle> getInnerRectangle() const;
    std::shared_ptr<Rectangle> getOuterRectangle() const;
    
private:
    Vec2 bottomLeft;
    Vec2 size;
    Vec2 margin;
};

}
