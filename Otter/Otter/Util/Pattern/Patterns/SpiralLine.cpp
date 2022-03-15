#include "SpiralLine.hpp"
#include "../../../Plottables/Path.hpp"

using namespace std;

namespace otter {

SpiralLine::SpiralLine(double angleOffset, double distanceFactor, int startIndex)
    : angleOffset(angleOffset)
    , distanceFactor(distanceFactor)
    , startIndex(startIndex)
{
}

Queue SpiralLine::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 center = bounds->getCenter();
    const double maxRadius = getMaxDistanceToCorner(center, bounds) * 1.05;
    
    double radius = 0.0;
    int i = startIndex;
    
    shared_ptr<Path> path = Path::create();
    while (radius < maxRadius)
    {
        radius = sqrt(i) * distanceFactor;
        path->add(center + Vec2::polar(i * angleOffset, radius));
        i++;
    }
    outQueue.add(path);
    
    return outQueue.getMaskedInside(bounds);
}

}
