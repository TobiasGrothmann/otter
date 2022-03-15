#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class SpiralLine : public GeneralPattern
{
public:
    SpiralLine(double angleOffset = 1.6180339887*2*M_PI, double distanceFactor = sqrt(2.0), int startIndex = 1);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double angleOffset;
    double distanceFactor;
    int startIndex;
};

}
