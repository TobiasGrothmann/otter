#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Path.hpp"
#include "../../Plottables/Rectangle.hpp"

namespace otter {

class Sample
{
public:
    constexpr Sample(const Vec2& point, double value)
        : point(point)
        , value(value)
    {
    }
    
    Vec2 point;
    double value;
};

class SampleTriangle
{
public:
    constexpr SampleTriangle(const Sample* a, const Sample* b, const Sample* c)
        : a(a)
        , b(b)
        , c(c)
        , minValue(std::min(std::min(a->value, b->value), c->value))
        , maxValue(std::max(std::max(a->value, b->value), c->value))
    {
    }
    
    const Sample* a = nullptr;
    const Sample* b = nullptr;
    const Sample* c = nullptr;
    
    double minValue;
    double maxValue;
    
    bool getIntersection(double targetValue, std::shared_ptr<Path>& outPath) const;
};

}
