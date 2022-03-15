#include "QuadTree.hpp"

using namespace std;

namespace otter {

QuadTree::QuadTree(const Queue& queue, int capacity, bool oversampled)
    : capacity(capacity)
{
    this->bounds = make_shared<Rectangle>(queue.getBounds());
    this->sector = make_shared<Sector>(this->bounds, capacity);
    for (const shared_ptr<Plottable>& plottable : queue.items)
    {
        if (oversampled)
            this->addOversampled(plottable);
        else
            this->add(plottable);
    }
}
QuadTree::QuadTree(const Rectangle& bounds, int capacity)
{
    this->capacity = capacity;
    this->bounds = make_shared<Rectangle>(bounds);
    this->sector = make_shared<Sector>(this->bounds, capacity);
}
QuadTree::QuadTree(const shared_ptr<Rectangle>& bounds, int capacity)
{
    this->capacity = capacity;
    this->bounds = bounds;
    this->sector = make_shared<Sector>(this->bounds, capacity);
}

void QuadTree::add(const Vec2& point)
{
    this->add(QuadTreeVec(point, nullptr));
}

void QuadTree::add(const vector<Vec2>& points)
{
    for (const Vec2& point : points)
    {
        this->add(QuadTreeVec(point, nullptr));
    }
}

void QuadTree::add(const QuadTreeVec& point)
{
    this->sector->add(point);
}

void QuadTree::add(const shared_ptr<Plottable>& plottable, const SampleSettings& sampleSettings)
{
    const vector<Vec2> points = plottable->getPlottablePoints(sampleSettings);
    for (const Vec2& point : points)
    {
        this->add(QuadTreeVec(point, plottable));
    }
}

void QuadTree::addOversampled(const std::shared_ptr<Plottable>& plottable, const SampleSettings& sampleSettings)
{
    const vector<Vec2> points = plottable->getOversampledPoints(sampleSettings);
    for (const Vec2& point : points)
    {
        this->add(QuadTreeVec(point, plottable));
    }
}


vector<QuadTreeVec> QuadTree::getPoints(const Rectangle& queryBounds) const
{
    vector<QuadTreeVec> output = {};
    query(this->sector, output, queryBounds);
    return output;
}

vector<QuadTreeVec> QuadTree::getAllPoints() const
{
    vector<QuadTreeVec> output = {};
    queryAll(this->sector, output);
    return output;
}

void QuadTree::query(const shared_ptr<Sector>& querySector, vector<QuadTreeVec>& found, const Rectangle& queryBounds) const
{
    if (!querySector->bounds->overlaps(queryBounds))
    {
        return;
    }
    for (const QuadTreeVec& point : querySector->points)
    {
        if (queryBounds.surrounds(point.point)) found.push_back(point);
    }
    if (querySector->divided)
    {
        query(querySector->bl, found, queryBounds);
        query(querySector->br, found, queryBounds);
        query(querySector->tl, found, queryBounds);
        query(querySector->tr, found, queryBounds);
    }
}

void QuadTree::queryAll(const shared_ptr<Sector>& querySector, vector<QuadTreeVec>& found) const
{
    for (const QuadTreeVec& point : querySector->points)
    {
        found.push_back(point);
    }
    if (querySector->divided)
    {
        queryAll(querySector->bl, found);
        queryAll(querySector->br, found);
        queryAll(querySector->tl, found);
        queryAll(querySector->tr, found);
    }
}


QuadTreeVec QuadTree::getClosestPoint(const Vec2& to, bool ignorePointItsself, const vector<shared_ptr<Plottable>>& ignored) const
{
    QuadTreeVec closestPoint;
    double closestDist = 999999999.9;
    
    queryClosest(this->sector, closestDist, closestPoint, to, ignorePointItsself, ignored);
    
    return closestPoint;
}

void QuadTree::queryClosest(const shared_ptr<Sector>& querySector, double& closestDistance, QuadTreeVec& closestPoint, const Vec2& targetPoint, bool ignorePointItsself, const vector<shared_ptr<Plottable>>& ignored) const
{
    if (querySector->divided)
    {
        map<double, shared_ptr<Sector>> children = {
            { getMinDistSquared(targetPoint, querySector->tl), querySector->tl },
            { getMinDistSquared(targetPoint, querySector->tr), querySector->tr },
            { getMinDistSquared(targetPoint, querySector->bl), querySector->bl },
            { getMinDistSquared(targetPoint, querySector->br), querySector->br },
        };
        
        for (map<double, shared_ptr<Sector>>::iterator child = children.begin(); child != children.end(); ++child)
        {
            if (child->first < closestDistance)
            {
                queryClosest(child->second, closestDistance, closestPoint, targetPoint, ignorePointItsself, ignored);
            }
        }
    }
    
    if (getMinDistSquared(targetPoint, querySector) < closestDistance)
    {
        for (const QuadTreeVec& point : querySector->points)
        {
            if (!ignored.empty() && find(ignored.begin(), ignored.end(), point.plottable) != ignored.end())
            {
                continue;
            }
            
            const double distSquared = point.point.distanceSquared(targetPoint);
            if (distSquared >= closestDistance) continue;
            if (ignorePointItsself && (targetPoint == point.point)) continue;
            
            closestDistance = distSquared;
            closestPoint = point;
        }
    }
}

double QuadTree::getMinDistSquared(const Vec2& point, const shared_ptr<Sector>& querySector) const
{
    if (querySector->bounds->surrounds(point)) return 0.0;
    
    const Vec2& botLeft = querySector->bounds->bottomLeft;
    const Vec2& topRight = querySector->bounds->topRight;
    
    if (point.x < botLeft.x) // left
    {
        if (point.y > topRight.y) // topLeft
        {
            return point.distanceSquared(Vec2(botLeft.x, topRight.y));
        }
        else if (point.y > botLeft.y) // left
        {
            return pow(botLeft.x - point.x, 2.0);
        }
        else // botLeft
        {
            return point.distanceSquared(botLeft);
        }
    }
    else if (point.x < topRight.x) // top/bot
    {
        if (point.y > topRight.y) // top
        {
            return pow(point.y - topRight.y, 2.0);
        }
        else // bot
        {
            return pow(botLeft.y - point.y, 2.0);
        }
    }
    else // right
    {
        if (point.y > topRight.y) // topRight
        {
            return point.distanceSquared(topRight);
        }
        else if (point.y > botLeft.y) // right
        {
            return pow(point.x - topRight.x, 2.0);
        }
        else // botRight
        {
            return point.distanceSquared(Vec2(topRight.x, botLeft.y));
        }
    }
}

}
