#include "Boolean.hpp"

using namespace std;

namespace otter {

Queue Boolean::getIntersection(const PlottablePtr& a, const PlottablePtr& b)
{
    return execute({a}, {b}, ClipperLib::ClipType::ctIntersection);
}
Queue Boolean::getUnion(const PlottablePtr& a, const PlottablePtr& b)
{
    return execute({a}, {b}, ClipperLib::ClipType::ctUnion);
}
Queue Boolean::getANotB(const PlottablePtr& a, const PlottablePtr& b)
{
    return execute({a}, {b}, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getBNotA(const PlottablePtr& a, const PlottablePtr& b)
{
    return execute({b}, {a}, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getXor(const PlottablePtr& a, const PlottablePtr& b)
{
    return execute({a}, {b}, ClipperLib::ClipType::ctXor);
}

Queue Boolean::getIntersection(const Queue& a, const Queue& b)
{
    return execute(a.items, b.items, ClipperLib::ClipType::ctIntersection);
}
Queue Boolean::getUnion(const Queue& a, const Queue& b)
{
    return execute(a.items, b.items, ClipperLib::ClipType::ctUnion);
}
Queue Boolean::getANotB(const Queue& a, const Queue& b)
{
    return execute(a.items, b.items, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getBNotA(const Queue& a, const Queue& b)
{
    return execute(b.items, a.items, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getXor(const Queue& a, const Queue& b)
{
    return execute(a.items, b.items, ClipperLib::ClipType::ctXor);
}

Queue Boolean::getIntersection(const PlottablePtr& a, const Queue& b)
{
    return execute({a}, b.items, ClipperLib::ClipType::ctIntersection);
}
Queue Boolean::getUnion(const PlottablePtr& a, const Queue& b)
{
    return execute({a}, b.items, ClipperLib::ClipType::ctUnion);
}
Queue Boolean::getANotB(const PlottablePtr& a, const Queue& b)
{
    return execute({a}, b.items, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getBNotA(const PlottablePtr& a, const Queue& b)
{
    return execute(b.items, {a}, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getXor(const PlottablePtr& a, const Queue& b)
{
    return execute({a}, b.items, ClipperLib::ClipType::ctXor);
}

Queue Boolean::getIntersection(const Queue& a, const PlottablePtr& b)
{
    return execute(a.items, {b}, ClipperLib::ClipType::ctIntersection);
}
Queue Boolean::getUnion(const Queue& a, const PlottablePtr& b)
{
    return execute(a.items, {b}, ClipperLib::ClipType::ctUnion);
}
Queue Boolean::getANotB(const Queue& a, const PlottablePtr& b)
{
    return execute(a.items, {b}, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getBNotA(const Queue& a, const PlottablePtr& b)
{
    return execute({b}, a.items, ClipperLib::ClipType::ctDifference);
}
Queue Boolean::getXor(const Queue& a, const PlottablePtr& b)
{
    return execute(a.items, {b}, ClipperLib::ClipType::ctXor);
}

Queue Boolean::execute(const vector<PlottablePtr>& a, const vector<PlottablePtr>& b, ClipperLib::ClipType type)
{
    Queue queue = Queue();
    
    ClipperLib::Clipper clipper = ClipperLib::Clipper();
    for (const PlottablePtr& p : a)
    {
        clipper.AddPath(ClipperConvert::pathFromPlottable(p), ClipperLib::PolyType::ptSubject, p->getIsClosed());
    }
    for (const PlottablePtr& p : b)
    {
        clipper.AddPath(ClipperConvert::pathFromPlottable(p), ClipperLib::PolyType::ptClip, p->getIsClosed());
    }
    
    ClipperLib::Paths solution = ClipperLib::Paths();
    const bool success = clipper.Execute(type, solution);
    if (!success)
    {
        cout << "Error: failed to get intersection" << endl;
        return queue;
    }
    
    for (const ClipperLib::Path& path : solution)
    {
        PathPtr plottable = ClipperConvert::plottableFromPath(path);
        plottable->close();
        queue.add(plottable);
    }
    queue.combinePlottables();
    return queue;
}

}
