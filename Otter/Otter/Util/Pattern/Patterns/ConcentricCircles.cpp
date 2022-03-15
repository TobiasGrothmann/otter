#include "ConcentricCircles.hpp"

using namespace std;

namespace otter {

ConcentricCircles::ConcentricCircles(double radiusDistance, const Vec2& centerFactor, double radiusOffset)
    : radiusDistance(radiusDistance)
    , centerFactor(centerFactor)
    , radiusOffset(radiusOffset)
{
}

Queue ConcentricCircles::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 center = bounds->getBottomLeft() + bounds->getSize() * centerFactor;
    const double maxRadius = getMaxDistanceToCorner(center, bounds) * 1.05;
    
    double radius = radiusDistance + radiusOffset;
    
    while (radius < maxRadius)
    {
        outQueue.circle(center, radius);
        radius += radiusDistance;
    }
    
    return outQueue.getMaskedInside(bounds);
}

}
