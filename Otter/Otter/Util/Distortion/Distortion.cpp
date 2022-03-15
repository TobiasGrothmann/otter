#include "Distortion.hpp"

using namespace std;

namespace otter {

Distortion::Distortion(std::function<Vec2(const Vec2&)> function)
    : function(function)
{
}

Queue Distortion::getDistorted(const Queue& queue, const SampleSettings& sampleSettings) const
{
    Queue outQueue = Queue();
    for (const shared_ptr<Plottable>& plottable : queue.items)
    {
        outQueue.add(getDistorted(plottable, sampleSettings));
    }
    return outQueue;
}

shared_ptr<Path> Distortion::getDistorted(const shared_ptr<Plottable>& plottable, const SampleSettings& sampleSettings) const
{
    shared_ptr<Path> path = Path::create();
    for (const Vec2& point : plottable->getOversampledPoints(sampleSettings))
    {
        path->add(function(point));
    }
    return path;
}

}
