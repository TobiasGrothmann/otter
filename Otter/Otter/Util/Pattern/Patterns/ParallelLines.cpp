#include "ParallelLines.hpp"
#include "../../../Plottables/Path.hpp"

using namespace std;

namespace otter {

ParallelLines::ParallelLines(double lineDistance, double angle, double offset)
    : lineDistance(lineDistance)
    , angle(angle)
    , offset(offset)
{
}

Queue ParallelLines::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 center = bounds->getCenter();
    const double diagonal = bounds->getBottomLeft().distance(bounds->getTopRight());
    const double drawingRadius = diagonal * 0.51 + abs(offset);
    
    const int numLines = (int) ceil((drawingRadius * 2) / lineDistance);
    for (int i = 0; i < numLines; i++)
    {
        const Vec2 onCenterLine = center + Vec2::polar(angle + M_PI * 0.5, -drawingRadius + i * lineDistance);
        const Vec2 lineOffset = Vec2::polar(angle, drawingRadius);
        outQueue.add(Path::create({ onCenterLine + lineOffset, onCenterLine - lineOffset }));
    }
    
    return outQueue.getMaskedInside(bounds);
}

}
