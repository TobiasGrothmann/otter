#include "Plottable.hpp"
#include "../Plottables/Path.hpp"
#include "../Plottables/Rectangle.hpp"
#include "../Plottables/Circle.hpp"
#include "../Math/Geometry.hpp"

using namespace std;

namespace otter {

Rectangle Plottable::getBounds() const
{
    Vec2 bottomLeft;
    Vec2 topRight;
    
    bool bInitialized = false;
    
    const vector<Vec2> points = getPlottablePoints(SampleSettings(10.0));
    for (const Vec2& point : points)
    {
        if (!bInitialized)
        {
            bottomLeft = point;
            topRight = point;
            bInitialized = true;
        }
        else
        {
            bottomLeft = Vec2(fmin(bottomLeft.x, point.x), fmin(bottomLeft.y, point.y));
            topRight = Vec2(fmax(topRight.x, point.x), fmax(topRight.y, point.y));
        }
    }
    
    if (!bInitialized)
    {
        cout << "WARNING: getting invalid Bounds of Plottable - returning zeros" << endl;
        return Rectangle(Vec2(0, 0), Vec2(0, 0));
    }
    
    return Rectangle(bottomLeft, topRight);
}

vector<Vec2> Plottable::getConvexHull(const SampleSettings& sampleSettings)
{
    const vector<Vec2> points = this->getPlottablePoints(sampleSettings);
    return Geometry::findConvexHull(points);
}

Plottable::Masked Plottable::mask(shared_ptr<Plottable> mask) const
{
    Masked output;
    
    const vector<Vec2> maskSegments = mask->getPlottablePoints();
    const vector<Vec2> segment = this->getPlottablePoints();
    
    if (segment.size() <= 0)
    {
        return output;
    }
    vector<Vec2> currentSegment = { segment[0] };
    bool currentlyInside = mask->surrounds(segment[0]);
    
    for (VINDEX(segment) i = 0; i < segment.size() - 1; i++) // segment points
    {
        vector<Vec2> intersections = Geometry::getAllIntersections(
                                      segment[i],
                                      segment[i+1],
                                      maskSegments);
        
        if (intersections.size() > 1) // sort by distance if more than one intersection
        {
            sort(intersections.begin(),
                 intersections.end(),
                 [i, segment](const Vec2& a, const Vec2& b) -> bool
                 { return segment[i].distanceSquared(a) < segment[i].distanceSquared(b); });
        }
        
        if (intersections.size() == 0)
        {
            currentSegment.push_back(segment[i + 1]);
        }
        else
        {
            for (const Vec2& intersection : intersections)
            {
                currentSegment.push_back(intersection);
                if (currentlyInside)
                {
                    output.inside.push_back(make_shared<Path>(currentSegment));
                }
                else
                {
                    output.outside.push_back(make_shared<Path>(currentSegment));
                }
                currentlyInside = !currentlyInside;
                currentSegment.clear();
                currentSegment.push_back(intersection);
            }
            currentSegment.push_back(segment[i + 1]);
        }
    }
    
    if (currentlyInside)
    {
        output.inside.push_back(make_shared<Path>(currentSegment));
    }
    else
    {
        output.outside.push_back(make_shared<Path>(currentSegment));
    }
    
    return output;
}

Plottable::Masked Plottable::mask(const vector<shared_ptr<Plottable>>& shapes)
{
    Masked output;
    for (const shared_ptr<Plottable>& shape : shapes)
    {
        Masked shapeMasked = shape->mask(shared_from_this());
        
        output.inside.insert(output.inside.end(), shapeMasked.inside.begin(), shapeMasked.inside.end());
        output.outside.insert(output.outside.end(), shapeMasked.outside.begin(), shapeMasked.outside.end());
    }
    return output;
}

vector<shared_ptr<Plottable>> Plottable::getMaskedOutside(const vector<shared_ptr<Plottable>>& shapes)
{
    vector<shared_ptr<Plottable>> output = { shared_from_this() };
    for (shared_ptr<Plottable> shape : shapes)
    {
        output = shape->mask(output).outside;
    }
    return output;
}

vector<shared_ptr<Plottable>> Plottable::getMaskedOutside(const vector<shared_ptr<Plottable>>& shapes, const vector<shared_ptr<Plottable>>& masks)
{
    vector<shared_ptr<Plottable>> output = {};
    for (shared_ptr<Plottable> shape : shapes)
    {
        vector<shared_ptr<Plottable>> newParts = shape->getMaskedOutside(masks);
        output.insert(output.end(), newParts.begin(), newParts.end());
    }
    return output;
}

vector<shared_ptr<Plottable>> Plottable::getMaskedInside(const vector<shared_ptr<Plottable>>& shapes)
{
    vector<shared_ptr<Plottable>> output;
    for (shared_ptr<Plottable> shape : shapes)
    {
        vector<shared_ptr<Plottable>> newParts = this->mask(shape).inside;
        output.insert(output.end(), newParts.begin(), newParts.end());
    }
    return output;
}

vector<shared_ptr<Plottable>> Plottable::getMaskedInside(const vector<shared_ptr<Plottable>>& shapes, const vector<shared_ptr<Plottable>>& masks)
{
    vector<shared_ptr<Plottable>> output = {};
    for (shared_ptr<Plottable> shape : shapes)
    {
        vector<shared_ptr<Plottable>> newParts = shape->getMaskedInside(masks);
        output.insert(output.end(), newParts.begin(), newParts.end());
    }
    return output;
}

void Plottable::scaleAt(const Vec2& pivot, double scale)
{
    this->offset(pivot * -1.0);
    this->scale(scale);
    this->offset(pivot);
}

void Plottable::rotate(const Vec2&, double)
{
    throw runtime_error("this plottable cannot be rotated");
}

void Plottable::normalize(const Rectangle& targetBounds, bool moveToMid, bool fillInBothDimensions)
{
    const Rectangle& shapeBounds = this->getBounds();
    const Vec2 scaleFactorVec2 = (targetBounds.getSize() / shapeBounds.getSize());
    const double scaleFactor = fillInBothDimensions ? scaleFactorVec2.max() : scaleFactorVec2.min();
    
    Vec2 offsetToCenter = Vec2(0, 0);
    if (moveToMid)
    {
        offsetToCenter = targetBounds.getSize() * 0.5 - shapeBounds.getSize() * 0.5 * scaleFactor;
    }
    
    this->offset(-1.0 * shapeBounds.getBottomLeft());
    this->scale(scaleFactor);
    this->offset(targetBounds.getBottomLeft() + offsetToCenter);
}

vector<Vec2> Plottable::getOversampledPoints(const SampleSettings& sampleSettings) const
{
    vector<Vec2> oversampledPoints = {};
    
    vector<Line> segments = getAsSegments(sampleSettings);
    if (segments.size() == 0) return oversampledPoints;
    
    for (VINDEX(segments) i = 0; i < segments.size(); i++)
    {
        const int num = max(1, (int) ceil(segments[i].getLength() * sampleSettings.resolution));
        for (int r = 0; r < num; r++)
        {
            const double factor = r / (double)num;
            oversampledPoints.push_back(segments[i].a.moveFactor(segments[i].b, factor));
        }
    }
    oversampledPoints.push_back(segments[segments.size() - 1].b);
    
    return oversampledPoints;
}

vector<Line> Plottable::getAsSegments(const SampleSettings& sampleSettings) const
{
    const vector<Vec2> points = getPlottablePoints(sampleSettings);
    vector<Line> segments = {};
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        segments.push_back(Line(points[i], points[i + 1]));
    }
    return segments;
}

shared_ptr<Plottable> Plottable::GetInvertedInCircle(const shared_ptr<Circle>& inversionCircle, const SampleSettings& sampleSettings) const
{
    const vector<Vec2> points = getOversampledPoints(sampleSettings);
    shared_ptr<Path> invertedPath = Path::create();
    invertedPath->points.reserve(points.size());
    for (const Vec2& point : points)
    {
        invertedPath->add(point.invert(inversionCircle));
    }
    return invertedPath;
}

}
