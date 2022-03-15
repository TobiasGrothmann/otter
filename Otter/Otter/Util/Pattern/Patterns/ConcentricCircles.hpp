#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class ConcentricCircles : public GeneralPattern
{
public:
    ConcentricCircles(double radiusDistance, const Vec2& centerFactor = Vec2(0.5, 0.5), double radiusOffset = 0.0);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double radiusDistance;
    Vec2 centerFactor;
    double radiusOffset;
};

}
