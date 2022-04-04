#pragma once
#include "stl.hpp"
#include "Vec2.hpp"

namespace otter {

/// line from point a to point b
class Line
{
public:
    Line(const Vec2& a, const Vec2& b);
    Line(const Vec2& a, double angle);
    
    /// get another line that is parallel to this one
    /// @param leftOffset distance to the left, negative if to the right
    Line getParallel(double leftOffset) const;
    
    /// get midpoint from a to b
    Vec2 getCenter() const;
    /// results in (b - a)
    Vec2 getDirection() const;
    /// get angle of (b - a)
    /// @param onlyPositive get only positive angles (0 to 2pi)
    double getAngle(bool onlyPositive) const;
    /// distance from a to b
    double getLength() const;
    
    Vec2 a;
    Vec2 b;
};

}
