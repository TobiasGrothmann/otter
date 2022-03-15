#include "ClipperConvert.hpp"

using namespace std;

namespace otter {

ClipperLib::cInt ClipperConvert::toClipperCoord(double value)
{
    return (ClipperLib::cInt)round(value * CLIPPER_RESOLUTION);
}
double ClipperConvert::fromClipperCoord(ClipperLib::cInt value)
{
    return (double)value / CLIPPER_RESOLUTION;
}

ClipperLib::IntPoint ClipperConvert::intPointFromVec2(const Vec2& point)
{
    return ClipperLib::IntPoint(ClipperConvert::toClipperCoord(point.x), ClipperConvert::toClipperCoord(point.y));
}
Vec2 ClipperConvert::vec2FromIntPoint(const ClipperLib::IntPoint point)
{
    return Vec2(ClipperConvert::fromClipperCoord(point.X), ClipperConvert::fromClipperCoord(point.Y));
}

ClipperLib::Path ClipperConvert::pathFromPlottable(const shared_ptr<Plottable>& plottable)
{
    const vector<Vec2> points = plottable->getPlottablePoints();
    ClipperLib::Path path = {};
    path.reserve(points.size());
    for (VINDEX(points) p = 0; p < points.size(); p++)
    {
        if (points[p] == points[(p + 1) % points.size()]) continue;
        path.push_back(ClipperConvert::intPointFromVec2(points[p]));
    }
    return path;
}

shared_ptr<Path> ClipperConvert::plottableFromPath(const ClipperLib::Path path)
{
    PathPtr plottablePath = Path::create();
    plottablePath->points.reserve(path.size());
    for (const ClipperLib::IntPoint& intPoint : path)
    {
        plottablePath->add(ClipperConvert::vec2FromIntPoint(intPoint));
    }
    return plottablePath;
}

ClipperLib::Paths ClipperConvert::pathsFromQueue(const Queue& queue)
{
    ClipperLib::Paths paths = {};
    paths.reserve(queue.items.size());
    for (const PlottablePtr& plottable : queue.items)
    {
        paths.push_back(pathFromPlottable(plottable));
    }
    return paths;
}

Queue ClipperConvert::queueFromPaths(const ClipperLib::Paths& paths)
{
    Queue queue = Queue();
    queue.items.reserve(paths.size());
    for (const ClipperLib::Path& path : paths)
    {
        queue.add(ClipperConvert::plottableFromPath(path));
    }
    return queue;
}

}
