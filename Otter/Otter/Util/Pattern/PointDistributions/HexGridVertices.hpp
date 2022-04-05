#pragma once
#include "../GeneralPointDistribution.hpp"

namespace otter {

/// generates the vertices of a hex grid, additionally the center is also added to essentially create the vertices for a triangular grid
class HexGridVertices : public GeneralPointDistribution
{
public:
    
    HexGridVertices(double lineDist, bool includeCenter = false);
    
    virtual std::vector<Vec2> create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double lineDist;
    bool includeCenter;
};

}
