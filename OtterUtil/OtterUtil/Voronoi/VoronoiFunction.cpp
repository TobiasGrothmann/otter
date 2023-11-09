#include "VoronoiFunction.hpp"

using namespace std;

namespace otter {

Voronoi VoronoiFunction::create(std::function<double(const Vec2& point)> function,
                                std::shared_ptr<Rectangle> clipBounds,
                                int iterations,
                                double moveToCentroidFactor)
{
    vector<Vec2> points = {};
    for (int i = 0; i < fmax(clipBounds->getArea(), 10); i++)
    {
        points.push_back(Vec2::random(clipBounds->getBottomLeft(), clipBounds->getTopRight()));
    }
    
    Voronoi* v = nullptr;
    for (int i = 0; i < iterations; i++)
    {
        v = new Voronoi(points, clipBounds);
        points.clear();
        for (VoronoiCell cell : v->cells)
        {
            vector<double> sampleValues = {function(cell.point)};
            for (const Vec2& point : cell.vertices)
            {
                sampleValues.push_back(function(point));
                sampleValues.push_back(function(cell.point.moveFactor(point, 0.5)));
            }
            double targetValue = 0.0;
            for (const double value : sampleValues)
            {
                targetValue += value / sampleValues.size();
            }
            
            double area = cell.createPath()->getArea();
            double actualValue = 1.0 / (area * 500.0);
            double valueError = targetValue - actualValue;
            
            Vec2 average = getWeightedAverage(cell, function);
            
            if (valueError < 0.2) continue; // remove
            else if (valueError > 0.8 && area > 0.01) // split
            {
                pair<Vec2, Vec2> splitPoints = getSplitCell(cell);
                points.push_back(splitPoints.first);
                points.push_back(splitPoints.second);
            }
            else // keep
            {
                points.push_back(cell.point.moveFactor(average, moveToCentroidFactor));
            }
        }
    }
    return *v;
}

pair<Vec2, Vec2> VoronoiFunction::getSplitCell(const VoronoiCell& cell)
{
    vector<Vec2> pointsA = {};
    vector<Vec2> pointsB = {};
    
    if (cell.vertices.size() % 2 != 0)
    {
        float half = cell.vertices.size() / 2.0;
        for (int i = 0; i < cell.vertices.size(); i++)
        {
            if (i < half) // a
            {
                pointsA.push_back(cell.vertices[i]);
                if (i + 1 > half) // next point in b
                {
                    // add point at half line
                    Vec2 halfwayPoint = cell.vertices[i].moveFactor(cell.vertices[i + 1], 0.5);
                    pointsA.push_back(halfwayPoint);
                    pointsB.push_back(halfwayPoint);
                }
            }
            else // b
            {
                pointsB.push_back(cell.vertices[i]);
            }
        }
        pointsB.push_back(cell.vertices[0]);
    }
    else
    {
        int half = (int)cell.vertices.size() / 2;
        for (int i = 0; i < cell.vertices.size(); i++)
        {
            if (i < half) // a
            {
                pointsA.push_back(cell.vertices[i]);
                if (i + 1 == half)
                {
                    // add next point too
                    pointsA.push_back(cell.vertices[i + 1]);
                }
            }
            else // b
            {
                pointsB.push_back(cell.vertices[i]);
            }
        }
        pointsB.push_back(cell.vertices[0]);
    }
    
    Vec2 a = Geometry::getAverageOfPoints(pointsA);
    Vec2 b = Geometry::getAverageOfPoints(pointsB);
    return {a, b};
}

PathPtr VoronoiFunction::getDot(const Vec2& point)
{
    double maxRadius = 0.004;
    double radius = 0.0;
    double angle = 0.0;
    PathPtr p = Path::create();
    while (radius < maxRadius)
    {
        p->add(point + Vec2::polar(angle, radius));
        angle += (M_PI * 2.0) / 8.0;
        radius += maxRadius / 16.0;
    }
    for (int i = 0; i < 15; i++)
    {
        p->add(point + Vec2::polar(angle, maxRadius));
        angle += (M_PI * 2.0) / 10.0;
    }
    return p;
}

Vec2 VoronoiFunction::getWeightedAverage(const VoronoiCell& cell, function<double(const Vec2& point)> func)
{
    Vec2 average = Geometry::getAverageOfPoints(cell.vertices);
    vector<Vec2> offsets = {};
    const double value = func(cell.point);
    for (const Vec2& vertex : cell.vertices)
    {
        double valueAtVertex = func(vertex);
        double offsetFactor = valueAtVertex - value;
        Vec2 offset = (vertex - average) * offsetFactor;
        offsets.push_back(offset);
    }
    for (const Vec2& offset : offsets)
    {
        average += offset;
    }
    return average;
}

}
