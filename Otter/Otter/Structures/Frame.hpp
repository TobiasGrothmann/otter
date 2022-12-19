#pragma once
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Plottables/Rectangle.hpp"

namespace otter {

/// Frame defines a rectangular outer area with another rectangular area within it.
/// It's mostly a handy tool to define a paper size and a margin for the image that will be plotted within it.
///
/// e.g. for a paper of size A6 with a 1cm margin:
/// \code{.cpp}
/// Frame f = Frame(Vec2::a6, 1.0);
/// RectanglePtr plotArea = f.getInnerRectangle();
/// \endcode
class Frame {
public:
    Frame(const Vec2& bottomLeft, const Vec2& size, const Vec2& margin);
    Frame(const Vec2& bottomLeft, const Vec2& size, double margin);
    Frame(const Vec2& size, const Vec2& margin);
    Frame(const Vec2& size, double margin);
    
    std::shared_ptr<Rectangle> getInnerRectangle() const;
    std::shared_ptr<Rectangle> getOuterRectangle() const;
    
    otter::Vec2 getCenter() const;
    
private:
    Vec2 bottomLeft;
    Vec2 size;
    Vec2 margin;
};

}
