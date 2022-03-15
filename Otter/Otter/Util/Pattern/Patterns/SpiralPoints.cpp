#include "SpiralPoints.hpp"

using namespace std;

namespace otter {

SpiralPoints::SpiralPoints(double circleSize, double angleOffset, double distanceFactor, int startIndex)
    : angleOffset(angleOffset)
    , circleSize(circleSize)
    , distanceFactor(distanceFactor)
    , startIndex(startIndex)
{
}

Queue SpiralPoints::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 center = bounds->getCenter();
    const double maxRadius = getMaxDistanceToCorner(center, bounds) * 1.05 + circleSize;
    
    double radius = 0.0;
    int i = startIndex;
    while (radius < maxRadius)
    {
        radius = sqrt(i) * distanceFactor;
        outQueue.circle(center + Vec2::polar(i * angleOffset, radius), circleSize);
        i++;
    }
    
    return outQueue.getMaskedInside(bounds);
}

}
