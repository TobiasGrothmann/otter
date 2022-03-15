#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class SpiralPoints : public GeneralPattern
{
public:
    SpiralPoints(double circleSize, double angleOffset = 1.6180339887*2*M_PI, double distanceFactor = sqrt(2.0), int startIndex = 1);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double angleOffset;
    double circleSize;
    double distanceFactor;
    int startIndex;
};

}
