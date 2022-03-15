#include "FlowField.hpp"

using namespace std;

namespace otter {

FlowField::FlowField(std::function<Vec2(const Vec2&)> function)
    : function(function)
{
}

Vec2 FlowField::sampleAt(const Vec2& point) const
{
    return function(point);
}

shared_ptr<Path> FlowField::walk(const Vec2& start, double toLength) const
{
    shared_ptr<Path> outPath = Path::create({ start });
    double length = 0.0;
    Vec2 previous = start;
    while (true)
    {
        const Vec2 next = function(previous);
        const double stepSize = previous.distance(next);
        length += stepSize;
        if (length > toLength)
        {
            const double desiredLength = (length - toLength);
            outPath->add(previous.moveDist(next, desiredLength));
            break;
        }
        outPath->add(next);
        previous = next;
    }
    return outPath;
}

}
