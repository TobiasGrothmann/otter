#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Rectangle.hpp"

namespace otter {

class GeneralPointDistribution
{
public:
    virtual std::vector<Vec2> create(const std::shared_ptr<Rectangle>& bounds) const = 0;
};

}
