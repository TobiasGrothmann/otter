#include "ConcentricSquares.hpp"

using namespace std;

namespace otter {

ConcentricSquares::ConcentricSquares(double lineDistance)
    : lineDistance(lineDistance)
{
}

Queue ConcentricSquares::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 center = bounds->getCenter();
    const double max = getMaxDistanceToCorner(center, bounds);
    
    double distance = lineDistance * 0.5;
    while (distance < max)
    {
        outQueue.rectangle(center - distance, center + distance);
        
        distance += lineDistance;
    }
    
    return outQueue.getMaskedInside(bounds);
}

}
