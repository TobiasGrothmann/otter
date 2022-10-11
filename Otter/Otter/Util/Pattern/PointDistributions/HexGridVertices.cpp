#include "HexGridVertices.hpp"

using namespace std;

namespace otter {

const static double HexFactor = sqrt(3.0);

HexGridVertices::HexGridVertices(double lineDist, bool includeCenter)
    : lineDist(lineDist)
    , includeCenter(includeCenter)
{
}

vector<Vec2> HexGridVertices::create(const shared_ptr<Rectangle>& bounds) const
{
    shared_ptr<Rectangle> usedBounds = Rectangle::create(bounds->getBottomLeft() - (lineDist * 2),
                                                         bounds->getTopRight() + (lineDist * 2));
    
    vector<Vec2> points = {};
    vector<Vec2> centers = {};
    points.reserve(getAproximateNumPointsForArea(usedBounds));
    centers.reserve(points.capacity() / 4);
    
    Vec2 pos = usedBounds->getBottomLeft();
    int yIndex = 0;
    while (pos.y < usedBounds->getTopRight().y)
    {
        pos.x = usedBounds->getBottomLeft().x;
        int xIndex = 0;
        while (pos.x < usedBounds->getTopRight().x)
        {
            Vec2 point = pos;
            
            if (xIndex % 2 == yIndex % 2)
            {
                point.y += lineDist * 0.5;
            }
            else if (includeCenter)
            {
                const Vec2 center = point + Vec2(0, lineDist);
                if (bounds->surrounds(center))
                    points.push_back(center);
            }
            
            if (bounds->surrounds(point))
                points.push_back(point);
            
            xIndex++;
            pos.x += lineDist * HexFactor * 0.5;
        }
        
        yIndex++;
        pos.y += lineDist * 1.5;
    }
    
    return points;
}

std::pair<std::vector<Vec2>, std::vector<Vec2>> HexGridVertices::createPointsAndCenters(const shared_ptr<Rectangle>& bounds) const
{
    shared_ptr<Rectangle> usedBounds = Rectangle::create(bounds->getBottomLeft() - (lineDist * 2),
                                                         bounds->getTopRight() + (lineDist * 2));
    
    vector<Vec2> points = {};
    vector<Vec2> centers = {};
    points.reserve(getAproximateNumPointsForArea(usedBounds));
    centers.reserve(points.capacity() / 4);
    
    Vec2 pos = usedBounds->getBottomLeft();
    int yIndex = 0;
    while (pos.y < usedBounds->getTopRight().y)
    {
        pos.x = usedBounds->getBottomLeft().x;
        int xIndex = 0;
        while (pos.x < usedBounds->getTopRight().x)
        {
            Vec2 point = pos;
            
            if (xIndex % 2 == yIndex % 2)
            {
                point.y += lineDist * 0.5;
            }
            else
            {
                const Vec2 center = point + Vec2(0, lineDist);
                if (bounds->surrounds(center))
                    centers.push_back(center);
            }
            
            if (bounds->surrounds(point))
                points.push_back(point);
            
            xIndex++;
            pos.x += lineDist * HexFactor * 0.5;
        }
        
        yIndex++;
        pos.y += lineDist * 1.5;
    }
    
    return { points, centers };
}

VINDEX_TYPE(Vec2) HexGridVertices::getAproximateNumPointsForArea(const std::shared_ptr<Rectangle>& bounds) const
{
    return (VINDEX_TYPE(Vec2))((bounds->getWidth() / (lineDist * HexFactor * 0.5)) *
                               (bounds->getHeight() / (lineDist * HexFactor * 1.5)));
}

}
