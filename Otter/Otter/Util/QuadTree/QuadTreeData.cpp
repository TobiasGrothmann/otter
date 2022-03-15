#include "QuadTreeData.hpp"
#include "../../Plottables/Rectangle.hpp"
#include "../../Plottables/Plottable.hpp"

using namespace std;

namespace otter {

Sector::Sector(const shared_ptr<Rectangle>& bounds, VINDEX_TYPE(QuadTreeVec) capacity)
    : capacity(capacity)
    , bounds(bounds)
{
    points.reserve(capacity);
}

void Sector::add(const QuadTreeVec& point)
{
    if (!bounds->surrounds(point.point))
    {
        return;
    }
    
    if (!divided && points.size() >= (VINDEX(points))capacity)
    {
        divide();
    }
    if (divided)
    {
        bl->add(point);
        br->add(point);
        tl->add(point);
        tr->add(point);
    }
    else
    {
        points.push_back(point);
    }
}

void Sector::divide()
{
    bl = make_shared<Sector>(make_shared<Rectangle>(
                                bounds->bottomLeft,
                                bounds->getCenter()),
                             capacity);
    br = make_shared<Sector>(make_shared<Rectangle>(
                                bounds->bottomLeft + bounds->getSize() * Vec2(0.5, 0.0),
                                bounds->bottomLeft + bounds->getSize() * Vec2(1.0, 0.5)),
                             capacity);
    tl = make_shared<Sector>(make_shared<Rectangle>(
                                bounds->bottomLeft + bounds->getSize() * Vec2(0.0, 0.5),
                                bounds->bottomLeft + bounds->getSize() * Vec2(0.5, 1.0)),
                             capacity);
    tr = make_shared<Sector>(make_shared<Rectangle>(
                                bounds->getCenter(),
                                bounds->topRight),
                             capacity);
    divided = true;
}

}
