#include "Line.hpp"

namespace otter {

Line::Line(const Vec2& a, const Vec2& b)
    : a(a)
    , b(b)
{
}

Line::Line(const Vec2& a, double angle)
    : a(a)
    , b(a + Vec2::polar(angle, 1.0))
{
}

Line Line::getParallel(double leftOffset) const
{
    const Vec2 offset = (b - a).normalLeft().normalize(leftOffset);
    return Line(a + offset, b + offset);
}

Vec2 Line::getCenter() const
{
    return a.moveFactor(b, 0.5);
}

Vec2 Line::getDirection() const
{
    return b - a;
};

double Line::getAngle(bool onlyPositive) const
{
    return (b - a).getAngle(onlyPositive);
}

double Line::getLength() const
{
    return a.distance(b);
}

}
