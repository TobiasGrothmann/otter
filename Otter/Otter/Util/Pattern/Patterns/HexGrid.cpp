#include "HexGrid.hpp"
#include "../../../Plottables/Path.hpp"

using namespace std;

namespace otter {

HexGrid::HexGrid(double lineDist)
    : lineDist(lineDist)
{
}

Queue HexGrid::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    Queue shortLines = Queue();
    
    const double factor = sqrt(3.0);
    
    shared_ptr<Rectangle> usedBounds = Rectangle::create(bounds->getBottomLeft() - (lineDist * 4),
                                                bounds->getTopRight() + (lineDist * 4));
    
    Vec2 pos = usedBounds->getBottomLeft();
    int yIndex = 0;
    while (pos.y < usedBounds->getTopRight().y)
    {
        shared_ptr<Path> path = Path::create();
        pos.x = usedBounds->getBottomLeft().x;
        int xIndex = 0;
        while (pos.x < usedBounds->getTopRight().x)
        {
            Vec2 pathPoint = pos;
            
            if (xIndex % 2 == yIndex % 2)
            {
                pathPoint += Vec2(0, lineDist * 0.5);
                shortLines.path({ pathPoint, pathPoint + Vec2(0, lineDist) });
            }
            path->add(pathPoint);
            
            xIndex++;
            pos.x += lineDist * factor * 0.5;
        }
        outQueue.add(path);
        
        yIndex++;
        pos.y += lineDist * 1.5;
    }
    
    outQueue.add(shortLines);
    
    return outQueue.getMaskedInside(bounds);
}

}
