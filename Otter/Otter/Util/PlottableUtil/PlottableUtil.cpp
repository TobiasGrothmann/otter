#include "PlottableUtil.hpp"
#include "../../Math/Math.hpp"

using namespace std;

namespace otter {

shared_ptr<Path> PlottableUtil::getFractionOfPlottable(const shared_ptr<Plottable> plottable, double startFraction, double endFraction, const SampleSettings& sampleSettings)
{
    shared_ptr<Path> path = Path::create();
    const vector<Vec2>& allPoints = plottable->getPlottablePoints(sampleSettings);
    if (allPoints.size() <= 1) return path;
    
    double totalLength = 0;
    for (VINDEX(allPoints) p = 0; p < allPoints.size() - 1; p++)
    {
        totalLength += allPoints[p].distance(allPoints[p + 1]);
    }
    
    const double startLength = totalLength * Math::clamp(min(endFraction, startFraction), 0.0, 1.0);
    const double endLength = totalLength * Math::clamp(max(endFraction, startFraction), 0.0, 1.0);
    
    double distanceAlongShape = 0.0;
    bool started = false;
    for (VINDEX(allPoints) i = 0; i < allPoints.size(); i++)
    {
        const double distanceToNext = allPoints[i].distance(allPoints[i + 1]);
        const double distanceAtNext = distanceAlongShape + distanceToNext;
        
        if (distanceAtNext >= startLength && !started) // need to start
        {
            started = true;
            const double distToStart = startLength - distanceAlongShape;
            path->add(allPoints[i].moveDist(allPoints[i + 1], distToStart));
            if (distanceAtNext > endLength) // fraction is only between two points
            {
                const double distToEnd = distanceAtNext - endLength;
                path->add(allPoints[i].moveDist(allPoints[i + 1], distToEnd));
                return path;
            }
            else // we started and the next point is still in
            {
                path->add(allPoints[i + 1]);
            }
        }
        else if (distanceAtNext > endLength && started)
        {
            if (i == allPoints.size() - 1) // at the very end
            {
                path->add(allPoints[i]);
                return path;
            }
            path->add(allPoints[i]);
            const double distToEnd = distanceAtNext - endLength;
            path->add(allPoints[i].moveDist(allPoints[i + 1], distToEnd));
            return path;
        }
        else if (started) // we are in the middle
        {
            path->add(allPoints[i]);
        }
        distanceAlongShape = distanceAtNext;
    }
    
    return path;
}

vector<Vec2> PlottableUtil::getPointsAlongPlottable(const shared_ptr<Plottable> plottable, double desiredStepSize, const SampleSettings& sampleSettings)
{
    const vector<Vec2>& allPoints = plottable->getPlottablePoints(sampleSettings);
    if (allPoints.size() <= 1) return {};
    
    double totalLength = 0;
    for (VINDEX(allPoints) p = 0; p < allPoints.size() - 1; p++)
    {
        totalLength += allPoints[p].distance(allPoints[p + 1]);
    }
    
    int steps = (int)round(totalLength / desiredStepSize);
    const double usedDistance = totalLength / steps;
    
    vector<Vec2> outPoints = { allPoints[0] };
    outPoints.reserve((VINDEX(outPoints))steps);
    
    double lengthAlongShape = 0.0;
    VINDEX(allPoints) shapePointIndex = 0;
    for (int s = 1; s < steps; s++)
    {
        const double targetDistance = s * usedDistance;
        
        while (true)
        {
            const double distToNext = allPoints[shapePointIndex].distance(allPoints[shapePointIndex + 1]);
            const double distanceFromPoint = targetDistance - lengthAlongShape;
            if (distanceFromPoint <= distToNext) // point is along this segment
            {
                outPoints.push_back(allPoints[shapePointIndex].moveDist(allPoints[shapePointIndex + 1], distanceFromPoint));
                break;
            }
            else // point is not in this segment
            {
                lengthAlongShape += distToNext;
                shapePointIndex++;
            }
        }
    }
    if (!plottable->getIsClosed())
    {
        outPoints.push_back(allPoints[allPoints.size() - 1]);
    }
    
    return outPoints;
}

Vec2 PlottableUtil::getPointAlongPlottableFactor(const shared_ptr<Plottable> plottable, double factor, const SampleSettings& sampleSettings)
{
    const double totalDistance = plottable->getLength();
    const double targetDistance = totalDistance * Math::clamp(factor, 0.0, 1.0);
    return PlottableUtil::getPointAlongPlottableDistance(plottable, targetDistance, sampleSettings);
}

Vec2 PlottableUtil::getPointAlongPlottableDistance(const shared_ptr<Plottable> plottable, double targetDistance, const SampleSettings& sampleSettings)
{
    const vector<Vec2> points = plottable->getPlottablePoints(sampleSettings);
    double currentDistance = 0.0;
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        const double segmentLength = points[i].distance(points[i + 1]);
        const double nextDistance = currentDistance + segmentLength;
        if (nextDistance >= targetDistance) // found the target in this segment
        {
            double distanceLeft = targetDistance - currentDistance;
            double segmentFactor = distanceLeft / segmentLength;
            return points[i].moveFactor(points[i + 1], segmentFactor);
        }
        currentDistance = nextDistance;
    }
    cout << "WARNING: failed to find point along shape" << endl;
    return Vec2();
}


Queue PlottableUtil::getDashed(const shared_ptr<Plottable> plottable, double desiredDistance, const SampleSettings& sampleSettings)
{
    Queue outQueue = Queue();
    
    const vector<Vec2>& allPoints = plottable->getPlottablePoints(sampleSettings);
    if (allPoints.size() <= 1) return {};
    
    double totalLength = 0;
    for (VINDEX(allPoints) p = 0; p < allPoints.size() - 1; p++)
    {
        totalLength += allPoints[p].distance(allPoints[p + 1]);
    }
    
    bool isClosed = plottable->getIsClosed();
    
    int steps = (int)round(totalLength / desiredDistance);
    if (isClosed)
    {
        if (steps % 2 == 1) steps++;
    }
    else
    {
        if (steps % 2 == 0) steps++;
    }
    
    const double usedDistance = totalLength / steps;
    
    
    double lengthAlongShape = 0.0;
    VINDEX(allPoints) shapePointIndex = 0;
    
    shared_ptr<Path> path = Path::create({ allPoints[0] });
    bool isDash = true;
    for (int s = 1; s < steps; s++)
    {
        const double targetDistance = s * usedDistance;
        
        while (true)
        {
            const double distToNext = allPoints[shapePointIndex].distance(allPoints[shapePointIndex + 1]);
            const double distanceFromPoint = targetDistance - lengthAlongShape;
            if (distanceFromPoint <= distToNext) // point is along this segment
            {
                const Vec2 targetPoint = allPoints[shapePointIndex].moveDist(allPoints[shapePointIndex + 1], distanceFromPoint);
                if (isDash)
                {
                    path->add(targetPoint);
                    outQueue.add(path);
                    isDash = false;
                }
                else
                {
                    path = Path::create({ targetPoint });
                    isDash = true;
                }
                break;
            }
            else // point is not in this segment
            {
                if (isDash)
                {
                    path->add(allPoints[shapePointIndex + 1]);
                }
                lengthAlongShape += distToNext;
                shapePointIndex++;
            }
        }
    }
    
    if (!isClosed && isDash && path->points.size() >= 1)
    {
        path->add(allPoints[allPoints.size() - 1]);
        outQueue.add(path);
    }
    
    return outQueue;
}

 
Queue PlottableUtil::getDotted(const shared_ptr<Plottable> plottable, double desiredDistance, const SampleSettings& sampleSettings)
{
    Queue outQueue = Queue();
    for (const Vec2& point : getPointsAlongPlottable(plottable, desiredDistance, sampleSettings))
    {
        outQueue.dot(point);
    }
    return outQueue;
}

Queue PlottableUtil::createKaleido(Queue input, const Vec2& center, double num, bool mask, double angleOffset)
{
    Queue output = Queue();
    int numCopies = (int)floor(num);
    
    if (mask)
    {
        const double maxDist = 10000.0;
        input = input.getMaskedInside(Path::create({Vec2(0, 0), Vec2(maxDist, 0), Vec2::polar((2.0*M_PI)/num, maxDist), Vec2(0, 0)}));
    }
    
    for (int i = 0; i < numCopies; i++)
    {
        double angle = i * ((2.0*M_PI) / num) + angleOffset;
        for (const shared_ptr<Plottable>& item : input.items)
        {
            shared_ptr<Plottable> copy = item->copy();
            copy->rotate(center, angle);
            output.add(copy);
        }
    }
    
    return output;
}

}
