#include "Filling.hpp"
#include "../../Plottables/Rectangle.hpp"
#include "../../Plottables/Path.hpp"

using namespace std;

namespace otter {

Queue Filling::getSimpleFilling(const shared_ptr<Plottable>& shape, double lineDist)
{
    Queue outQueue = Queue();
    const Rectangle bounds = shape->getBounds();
    
    if (!shape->getIsClosed())
        cout << "WARNING: getting simple filling for non closed Plottable";
    
    const int numLines = (int) ceil(bounds.getHeight() / lineDist);
    const double lineDistUsed = bounds.getHeight() / numLines;
    
    const double padding = 0.1;
    for (int i = 0; i < numLines + 1; i++)
    {
        double y = bounds.getBottomLeft().y + i * lineDistUsed;
        shared_ptr<Path> line = Path::create({
            Vec2(bounds.bottomLeft.x - padding, y),
            Vec2(bounds.topRight.x + padding, y)
        });
        outQueue.add(line->mask(shape).inside);
    }
    
    return outQueue;
}

}
