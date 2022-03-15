#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Path.hpp"

namespace otter {

class FlowField
{
public:
    FlowField(std::function<Vec2(const Vec2&)> function);
    
    Vec2 sampleAt(const Vec2& point) const;
    std::shared_ptr<Path> walk(const Vec2& start, double length) const;

private:
    std::function<Vec2(const Vec2&)> function;
};

}
