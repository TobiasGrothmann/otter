#pragma once
#include "../GeneralPointDistribution.hpp"

namespace otter {

/// generates the vertices of a hex grid, additionally the center is also added to essentially create the vertices for a triangular grid
class HexGridVertices : public GeneralPointDistribution
{
public:
    
    HexGridVertices(double lineDist, bool includeCenter = false);
    
    virtual std::vector<Vec2> create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    std::pair<std::vector<Vec2>, std::vector<Vec2>> createPointsAndCenters(const std::shared_ptr<Rectangle>& bounds) const;
    
    double lineDist;
    bool includeCenter;
    
private:
    VINDEX_TYPE(Vec2) getAproximateNumPointsForArea(const std::shared_ptr<Rectangle>& bounds) const;
};

}
