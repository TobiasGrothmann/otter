#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class RandomDots : public GeneralPattern
{
public:
    RandomDots(double dotDist, double dotSize = 0.001, double randomness = 1.0);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double dotDist;
    double dotSize;
    double randomness;
};

}
