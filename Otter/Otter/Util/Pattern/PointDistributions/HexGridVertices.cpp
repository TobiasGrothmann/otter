#include "HexGridVertices.hpp"

using namespace std;

namespace otter {

HexGridVertices::HexGridVertices(double lineDist, bool includeCenter)
    : lineDist(lineDist)
    , includeCenter(includeCenter)
{
}

vector<Vec2> HexGridVertices::create(const shared_ptr<Rectangle>& bounds) const
{
    const double factor = sqrt(3.0);
    
    shared_ptr<Rectangle> usedBounds = Rectangle::create(bounds->getBottomLeft() - (lineDist * 2),
                                                bounds->getTopRight() + (lineDist * 2));
    
    vector<Vec2> points = {};
    points.reserve((VINDEX_TYPE(Vec2))((usedBounds->getWidth() / (lineDist * factor * 0.5)) * (usedBounds->getHeight() / (lineDist * factor * 1-5))));
    
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
                Vec2 center = point + Vec2(0, lineDist);
                if (bounds->surrounds(center))
                {
                    points.push_back(center);
                }
            }
            
            if (bounds->surrounds(point))
            {
                points.push_back(point);
            }
            
            xIndex++;
            pos.x += lineDist * factor * 0.5;
        }
        
        yIndex++;
        pos.y += lineDist * 1.5;
    }
    
    return points;
}

}
