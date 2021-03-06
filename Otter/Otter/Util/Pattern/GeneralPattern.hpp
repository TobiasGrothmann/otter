#pragma once
#include "stl.hpp"
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Rectangle.hpp"

namespace otter {

/// parent class for the generation of patterns within given bounds
class GeneralPattern
{
public:
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const = 0;
    
protected:
    static double getMaxDistanceToCorner(const Vec2& point, const std::shared_ptr<Rectangle>& rect);
};

}
