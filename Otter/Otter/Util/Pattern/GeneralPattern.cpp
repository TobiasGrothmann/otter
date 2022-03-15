#include "GeneralPattern.hpp"

using namespace std;

namespace otter {

double GeneralPattern::getMaxDistanceToCorner(const Vec2& point, const shared_ptr<Rectangle>& rect)
{
    double distancesToCorners[4] = {
        rect->getBottomLeft().distance(point),
        rect->getTopLeft().distance(point),
        rect->getTopRight().distance(point),
        rect->getBottomRight().distance(point),
    };
    
    double maxDistance = distancesToCorners[0];
    for (int i = 1; i < 4; i++)
    {
        if (distancesToCorners[i] > maxDistance)
        {
            maxDistance = distancesToCorners[i];
        }
    }
    
    return maxDistance;
}

}
