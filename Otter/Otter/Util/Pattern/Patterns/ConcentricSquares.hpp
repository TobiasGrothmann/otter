#pragma once
#include "stl.hpp"
#include "../GeneralPattern.hpp"

namespace otter {

class ConcentricSquares : public GeneralPattern
{
public:
    ConcentricSquares(double lineDistance);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double lineDistance;
};

}
