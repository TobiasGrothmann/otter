#pragma once
#include "../GeneralPointDistribution.hpp"

namespace otter {

class HexGridVertices : public GeneralPointDistribution
{
public:
    
    HexGridVertices(double lineDist, bool includeCenter = false);
    
    virtual std::vector<Vec2> create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double lineDist;
    bool includeCenter;
};

}
