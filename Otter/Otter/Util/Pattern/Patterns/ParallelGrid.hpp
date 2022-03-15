#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class ParallelGrid : public GeneralPattern
{
public:
    ParallelGrid(double lineDistance, double angle, double offset = 0.0);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double lineDistance;
    double angle;
    double offset;
};

}
