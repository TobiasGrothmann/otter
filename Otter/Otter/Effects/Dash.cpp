#include "Dash.hpp"

#include "../Geometry/Vec2.hpp"
#include "../Plottables/Path.hpp"
#include "../Rand/Rand.hpp"

using namespace std;

namespace otter {

Queue Dash::dash(const std::shared_ptr<Plottable> plottable,
                 const std::function<double(const Vec2& point,
                                            bool isGap,
                                            double pathDistance,
                                            double pahtFraction)>& func)
{
    Queue dashed = Queue();
    const vector<Vec2> points = plottable->getPlottablePoints(SampleSettings(20));
    
    if (points.size() == 0)
        return dashed;
    
    const double pathTotalDistance = plottable->getLength();
    
    double currentDistance = 0.0;
    shared_ptr<Path> currentPath = Path::create({ points[0] });
    double currentDashLength = 0.0;
    
    VINDEX(points) nextIndex = 1;
    Vec2 lastUsedPoint = points[0];
    
    int dashIndex = 0;
    
    while (nextIndex < points.size())
    {
        const Vec2& nextPoint = points[nextIndex];
        double distanceToNextPoint = lastUsedPoint.distance(nextPoint);

        const double currentFraction = currentDistance / pathTotalDistance;
        const double desiredDashLength = max(0.01, func(lastUsedPoint, dashIndex % 2 != 0, currentDistance, currentFraction));
        
        const double dashLengthAtNextPoint = currentDashLength + distanceToNextPoint;
        
        if (dashLengthAtNextPoint > desiredDashLength) // next point is too far
        {
            const double dashLengthLeftNeeded = desiredDashLength - currentDashLength;
            const Vec2 pointToGoTo = lastUsedPoint.moveDist(nextPoint, dashLengthLeftNeeded);
            
            if (dashIndex % 2 == 0)
            {
                currentPath->add(pointToGoTo);
                dashed.add(currentPath);
            }
            dashIndex++;
            
            currentPath = Path::create();
            currentPath->add(pointToGoTo);
            currentDashLength = 0.0;

            lastUsedPoint = pointToGoTo;
            currentDistance += dashLengthLeftNeeded; // TODO check
        }
        else // include next point
        {
            currentPath->add(nextPoint);
            currentDashLength = dashLengthAtNextPoint;
            nextIndex++;
            
            lastUsedPoint = nextPoint;
            currentDistance += distanceToNextPoint;
        }
    }
    
    if (currentPath->points.size() >= 2 && dashIndex % 2 == 0)
    {
        currentPath->add(points[points.size() - 1]);
        dashed.add(currentPath);
    }
    
    return dashed;
}

Queue Dash::dash(const std::shared_ptr<Plottable> plottable,
                 double dashLength,
                 double gapLength)
{
    return dash(plottable, [dashLength, gapLength](const Vec2& point,
                                                   bool isGap,
                                                   double pahtDistance,
                                                   double pathFraction) -> double
    {
        return isGap ? gapLength : dashLength;
    });
}

Queue Dash::dash(const Queue& queue,
                 double penWidth,
                 const std::function<double(const Vec2& point,
                                             bool isGap,
                                             double pathDistance,
                                             double pathFraction)>& func)
{
    Queue dashedQueue = Queue();
    dashedQueue.items.reserve(queue.items.size());
    for (const shared_ptr<Plottable>& p : queue.items)
    {
        dashedQueue.add(dash(p, func));
    }
    return dashedQueue;
}

Queue Dash::dash(const Queue& queue,
                 double dashLength,
                 double gapLength)
{
    Queue dashedQueue = Queue();
    dashedQueue.items.reserve(queue.items.size());
    for (const shared_ptr<Plottable>& p : queue.items)
    {
        dashedQueue.add(dash(p, dashLength, gapLength));
    }
    return dashedQueue;
}

}
