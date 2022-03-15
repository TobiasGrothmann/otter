#pragma once
#include "stl.hpp"
#include "Vec2.hpp"

namespace otter {

class Line
{
public:
    Line(const Vec2& a, const Vec2& b);
    Line(const Vec2& a, double angle);
    
    Line getParallel(double leftOffset) const;
    
    Vec2 getCenter() const;
    Vec2 getDirection() const;
    double getAngle(bool onlyPositive) const;
    double getLength() const;
    
    Vec2 a;
    Vec2 b;
};

}
