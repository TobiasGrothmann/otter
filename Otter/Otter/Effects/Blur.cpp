#include "Blur.hpp"

#include "../Geometry/Vec2.hpp"
#include "../Plottables/Path.hpp"
#include "../Rand/Rand.hpp"

using namespace std;

namespace otter {

Queue Blur::blur(const shared_ptr<Plottable> plottable,
                 double penWidth,
                 const std::function<double(const Vec2& point,
                                            double pathDistance,
                                            double pathFraction)>& func)
{
    const double interval = (1.0 / penWidth) * 1.9;
    
    Queue blurred = Queue();
    const vector<Vec2> points = plottable->getOversampledPoints(SampleSettings(interval));
    
    shared_ptr<Path> currentPath = Path::create();
    double pathDistance = 0.0;
    const double pathTotalDistance = plottable->getLength();
    
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        if (i != 0)
        {
            pathDistance += points[i - 1].distance(points[i]);
        }
        const double pathFraction = pathDistance / pathTotalDistance;
        const double blurDistance = func(points[i], pathDistance, pathFraction);
        const Vec2 newPoint = points[i] + Vec2::polar(Rand::angle(), Rand::normalDistValue(0.0, blurDistance));
        
        Vec2 lastPoint;
        bool isFirstPoint = !currentPath->getLastPoint(lastPoint);
        if (isFirstPoint)
        {
            currentPath->add(newPoint);
        }
        else
        {
            if (lastPoint.distance(newPoint) < penWidth) // very close point
            {
                currentPath->add(newPoint);
            }
            else // far point
            {
                if (currentPath->points.size() == 1) // is only dot
                {
                    blurred.dot(lastPoint);
                }
                else // is longer path
                {
                    blurred.add(currentPath);
                }
                currentPath = Path::create({ newPoint });
            }
        }
    }
    
    if (currentPath->points.size() == 1) // is only dot
    {
        blurred.dot(currentPath->points[0]);
    }
    else // is longer path
    {
        blurred.add(currentPath);
    }
    return blurred;
}

Queue Blur::blur(const shared_ptr<Plottable> plottable,
                 double penWidth,
                 double blurAmount)
{
    return blur(plottable, penWidth, [blurAmount](const Vec2&, double, double) -> double { return blurAmount; });
}

Queue Blur::blur(const Queue& queue,
                 double penWidth,
                 const std::function<double(const Vec2& point,
                                            double pathDistance,
                                            double pathFraction)>& func)
{
    Queue blurredQueue = Queue();
    blurredQueue.items.reserve(queue.items.size());
    for (const shared_ptr<Plottable>& p : queue.items)
    {
        blurredQueue.add(blur(p, penWidth, func));
    }
    return blurredQueue;
}

Queue Blur::blur(const Queue& queue,
                 double penWidth,
                 double blurAmount)
{
    Queue blurredQueue = Queue();
    blurredQueue.items.reserve(queue.items.size());
    for (const shared_ptr<Plottable>& p : queue.items)
    {
        blurredQueue.add(blur(p, penWidth, blurAmount));
    }
    return blurredQueue;
}
    
}
