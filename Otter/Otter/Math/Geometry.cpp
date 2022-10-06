#include "Geometry.hpp"
#include "Math.hpp"
#include "../Plottables/Rectangle.hpp"

using namespace std;

namespace otter {

bool Geometry::getIntersection(const Vec2 &aStart,
                               const Vec2 &aEnd,
                               const Vec2 &bStart,
                               const Vec2 &bEnd,
                               Vec2& outVec,
                               bool checkIfOnLines)
{
    // Line AB represented as a1x + b1y = c1
    const double a1 = aEnd.y - aStart.y;
    const double b1 = aStart.x - aEnd.x;
    const double c1 = a1 * aStart.x + b1 * aStart.y;

    // Line CD represented as a2x + b2y = c2
    const double a2 = bEnd.y - bStart.y;
    const double b2 = bStart.x - bEnd.x;
    const double c2 = a2 * bStart.x + b2 * bStart.y;

    const double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) // the lines are paralell
    {
        return false;
    }
    
    const double x = (b2 * c1 - b1 * c2) / determinant;
    const double y = (a1 * c2 - a2 * c1) / determinant;
    const Vec2 intersection = Vec2(x, y);
    
    if (!checkIfOnLines)
    {
        outVec = intersection;
        return true;
    }
    
    const Vec2 minA = Vec2(fmin(aStart.x, aEnd.x), fmin(aStart.y, aEnd.y));
    const Vec2 maxA = Vec2(fmax(aStart.x, aEnd.x), fmax(aStart.y, aEnd.y));
    const Vec2 minB = Vec2(fmin(bStart.x, bEnd.x), fmin(bStart.y, bEnd.y));
    const Vec2 maxB = Vec2(fmax(bStart.x, bEnd.x), fmax(bStart.y, bEnd.y));
    
    const double errorMargin = 0.000000001;
    if (  // check wether the intersection is on both line-segments
        intersection.x >= minA.x - errorMargin &&
        intersection.y >= minA.y - errorMargin &&
        intersection.x <= maxA.x + errorMargin &&
        intersection.y <= maxA.y + errorMargin &&
        intersection.x >= minB.x - errorMargin &&
        intersection.y >= minB.y - errorMargin &&
        intersection.x <= maxB.x + errorMargin &&
        intersection.y <= maxB.y + errorMargin
        )
    {
        outVec = intersection;
        return true;
    }
    
    return false;
}


vector<Vec2> Geometry::getAllIntersections(const Vec2& lineStart,
                                           const Vec2& lineEnd,
                                           const vector<Vec2>& points)
{
    vector<Vec2> intersections = {};
    Vec2 foundIntersection;
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        if (Geometry::getIntersection(lineStart,
                                      lineEnd,
                                      points[i],
                                      points[i + 1],
                                      foundIntersection,
                                      true))
        {
            bool alreadyFoundThisIntersection = false;
            for (const Vec2& intersection : intersections)
            {
                if (intersection == foundIntersection)
                {
                    alreadyFoundThisIntersection = true;
                    break;
                }
            }
            
            if (!alreadyFoundThisIntersection)
            {
                intersections.push_back(foundIntersection);
            }
        }
    }
    return intersections;
}
vector<Intersection> Geometry::getAllIntersectionsWithSegment(const Vec2& lineStart,
                                                              const Vec2& lineEnd,
                                                              const vector<Vec2>& points)
{
    vector<Intersection> intersections = {};
    Vec2 foundIntersectionLocation;
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        if (Geometry::getIntersection(lineStart,
                                      lineEnd,
                                      points[i],
                                      points[i + 1],
                                      foundIntersectionLocation,
                                      true))
        {
            intersections.push_back({ foundIntersectionLocation, i });
        }
    }
    return intersections;
}




vector<Vec2> Geometry::getAllIntersections(const Vec2& lineStart,
                                           const Vec2& lineEnd,
                                           const Plottable* plottable)
{
    return Geometry::getAllIntersections(lineStart,
                                         lineEnd,
                                         plottable->getPlottablePoints());
}

vector<Intersection> Geometry::getAllIntersectionsWithSegment(const Vec2& lineStart,
                                                              const Vec2& lineEnd,
                                                              const Plottable* plottable)
{
    return Geometry::getAllIntersectionsWithSegment(lineStart,
                                                    lineEnd,
                                                    plottable->getPlottablePoints());
}



shared_ptr<Path> Geometry::getSmoothCorners(const shared_ptr<Plottable> shape,
                                            double factor,
                                            int iterations,
                                            double minDistManhattan)
{
    vector<Vec2> points = shape->getPlottablePoints();
    factor = Math::clamp(factor, 0.0, 0.5);
    
    const bool closed = shape->getIsClosed();
    for (int n = 0; n < iterations; n++)
    {
        vector<Vec2> newPoints = {};
        newPoints.reserve((VINDEX(newPoints))(points.size() * 1.5));
        if (!closed) // start is kept if not closed
        {
            newPoints.push_back(points[0]);
        }
        
        VINDEX(points) endIndex = closed ? points.size() - 1 : points.size();
        
        for (VINDEX(points) i = 0; i < endIndex; i++)
        {
            VINDEX(points) nextIndex = i + 1;
            if (closed && nextIndex == points.size()) nextIndex = 1; // first point is double in closed path
            
            if (points[i].distanceManhattan(points[nextIndex]) < minDistManhattan)
            {
                newPoints.push_back(points[i].moveFactor(points[nextIndex], 0.5));
                continue;
            }
            
            newPoints.push_back(points[i].moveFactor(points[nextIndex], factor));
            newPoints.push_back(points[nextIndex].moveFactor(points[i], factor));
        }
        if (!closed) // end is kept if not closed
        {
            newPoints.push_back(points[points.size() - 1]);
        }
        else // if originally closed, need to close again
        {
            newPoints.push_back(newPoints[0]);
        }
        
        // stop if no points were added
        if (newPoints.size() == points.size()) break;
        
        points = newPoints;
    }
    
    return Path::create(points);
}

shared_ptr<Path> Geometry::getCutCorners(const shared_ptr<Plottable> shape,
                                         double cutDist,
                                         int iterations)
{
    vector<Vec2> points = shape->getPlottablePoints();
    
    const bool closed = shape->getIsClosed();
    for (int n = 0; n < iterations; n++)
    {
        vector<Vec2> newPoints = {};
        newPoints.reserve((VINDEX(newPoints))(points.size() * 1.5));
        if (!closed) // start is kept if not closed
        {
            newPoints.push_back(points[0]);
        }
        
        VINDEX(points) endIndex = closed ? points.size() - 1 : points.size();
        
        for (VINDEX(points) i = 0; i < endIndex; i++)
        {
            VINDEX(points) nextIndex = i + 1;
            if (closed && nextIndex == points.size()) nextIndex = 1; // first point is double in closed path
            
            const double segmentLength = points[i].distance(points[i + 1]);
            if (segmentLength < cutDist * 2.001) // add middle if cut impossible
            {
                newPoints.push_back(points[i].moveFactor(points[nextIndex], 0.5));
            }
            else
            {
                newPoints.push_back(points[i].moveDist(points[nextIndex], cutDist));
                newPoints.push_back(points[nextIndex].moveDist(points[i], cutDist));
            }
        }
        if (!closed) // end is kept if not closed
        {
            newPoints.push_back(points[points.size() - 1]);
        }
        else // if originally closed, need to close again
        {
            newPoints.push_back(newPoints[0]);
        }
        
        // stop if no points were added
        if (newPoints.size() == points.size()) break;
        
        points = newPoints;
    }
    
    return Path::create(points);
}

Vec2 Geometry::getClosestPointOnCircle(const Vec2& center, double radius, Vec2 point)
{
    if (center == point)
    {
        return center + Vec2(radius, 0);
    }
    return center.moveDist(point, radius);
}

double Geometry::getDistanceToCircle(const Vec2& center, double radius, Vec2 point)
{
    return getClosestPointOnCircle(center, radius, point).distance(point);
}

double Geometry::getDistanceOfCircles(const Vec2& centerA, double radiusA, const Vec2& centerB, double radiusB)
{
    return centerA.distance(centerB) - radiusA - radiusB;
}

bool Geometry::isCircleCompletelyInOther(const Vec2& centerA, double radiusA, const Vec2& centerB, double radiusB)
{
    return (centerA.distance(centerB) + radiusA) < radiusB;
}

vector<Vec2> Geometry::findConvexHull(vector<Vec2> points)
{
    if (points.size() < 2) return {};
    auto compareByX = [](const Vec2& a, const Vec2& b) -> bool
    {
        return a.x < b.x;
    };
    sort(points.begin(), points.end(), compareByX);
    
    vector<Vec2> hull = { points[0] };
    set<const Vec2*> usedPoints = { };
    Vec2* prevPoint = &hull[hull.size() - 1];
    while (true)
    {
        Vec2* candidate = nullptr;
        Vec2 vectorToCandidate = {};
        for (Vec2& point : points)
        {
            if (point == *prevPoint) continue;
            if (usedPoints.find(&point) != usedPoints.end()) continue;
            if (!candidate)
            {
                candidate = &point;
                vectorToCandidate = *candidate - *prevPoint;
                continue;
            }

            if (vectorToCandidate.cross(*prevPoint - point) < 0.0)
            {
                candidate = &point;
                vectorToCandidate = *candidate - *prevPoint;
            }
        }
        if (!candidate) break;
        hull.push_back(*candidate);
        prevPoint = candidate;
        usedPoints.insert(candidate);
        if (*candidate == hull[0]) break;
        candidate = nullptr;
    }
    
    return hull;
}

bool Geometry::getCircleFromPoints(const Vec2& a, const Vec2& b, const Vec2& c, Vec2& center, double& radius)
{
    double ox = (min(min(a.x, b.x), c.x) + max(max(a.x, b.x), c.x)) / 2;
    double oy = (min(min(a.y, b.y), c.y) + max(max(a.y, b.y), c.y)) / 2;
    double ax = a.x - ox,  ay = a.y - oy;
    double bx = b.x - ox,  by = b.y - oy;
    double cx = c.x - ox,  cy = c.y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
    if (d == 0)
        return false;
    double x = ((ax*ax + ay*ay) * (by - cy) + (bx*bx + by*by) * (cy - ay) + (cx*cx + cy*cy) * (ay - by)) / d;
    double y = ((ax*ax + ay*ay) * (cx - bx) + (bx*bx + by*by) * (ax - cx) + (cx*cx + cy*cy) * (bx - ax)) / d;
    Vec2 p = Vec2(ox + x, oy + y);
    double r = max(max(p.distance(a), p.distance(b)), p.distance(c));
    center = p;
    radius = r;
    return true;
}

Vec2 Geometry::getAverageOfPoints(const vector<Vec2>& points)
{
    Vec2 average = Vec2(0, 0);
    for (const Vec2& point : points)
    {
        average += point / points.size();
    }
    return average;
}

}
