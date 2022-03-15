#include "Bezier.hpp"
#include "../Math/Geometry.hpp"
#include "Rectangle.hpp"
#include "../Serialize/Serializable.hpp"

using namespace std;

namespace otter {

Bezier::Bezier()
    : points({})
{
    
}
Bezier::Bezier(const shared_ptr<Bezier>& other)
    : points(other->points)
{
    
}
Bezier::Bezier(const vector<Vec2>& points)
    : points(points)
{
    
}

shared_ptr<Bezier> Bezier::create()
{
    return make_shared<Bezier>();
}
shared_ptr<Bezier> Bezier::create(const shared_ptr<Bezier>& other)
{
    return make_shared<Bezier>(other);
}
shared_ptr<Bezier> Bezier::create(const vector<Vec2>& points)
{
    return make_shared<Bezier>(points);
}


void Bezier::add(const Vec2& newPoint)
{
    this->points.push_back(newPoint);
}

void Bezier::add(const vector<Vec2>& newPoints)
{
    for (const Vec2& point : newPoints)
    {
        add(point);
    }
}

void Bezier::add(const Bezier& BezierToAppend, bool reversed)
{
    for (VINDEX(BezierToAppend.points) i = 0; i < BezierToAppend.points.size(); i++)
    {
        if (reversed)
        {
            add(BezierToAppend.points[BezierToAppend.points.size() - i - 1]);
        }
        else
        {
            add(BezierToAppend.points[i]);
        }
    }
}
void Bezier::add(const shared_ptr<Bezier>& BezierToAppend, bool reversed)
{
    for (VINDEX(BezierToAppend->points) i = 0; i < BezierToAppend->points.size(); i++)
    {
        if (reversed)
        {
            add(BezierToAppend->points[BezierToAppend->points.size() - i - 1]);
        }
        else
        {
            add(BezierToAppend->points[i]);
        }
    }
}

void Bezier::addFront(Vec2 newPoint)
{
    this->points.insert(this->points.begin(), newPoint);
}
void Bezier::addFront(const Bezier& BezierToAppend, bool reversed)
{
    for (VINDEX(BezierToAppend.points) i = 0; i < BezierToAppend.points.size(); i++)
    {
        if (reversed)
        {
            addFront(BezierToAppend.points[i]);
        }
        else
        {
            addFront(BezierToAppend.points[BezierToAppend.points.size() - i - 1]);
        }
    }
}
void Bezier::addFront(const shared_ptr<Bezier>& BezierToAppend, bool reversed)
{
    for (VINDEX(BezierToAppend->points) i = 0; i < BezierToAppend->points.size(); i++)
    {
        if (reversed)
        {
            addFront(BezierToAppend->points[i]);
        }
        else
        {
            addFront(BezierToAppend->points[BezierToAppend->points.size() - i - 1]);
        }
    }
}



void Bezier::reverse()
{
    std::reverse(points.begin(), points.end());
}

void Bezier::close()
{
    if (!getIsClosed() && points.size() > 1)
    {
        this->points.push_back(points[0]);
    }
}

bool Bezier::getLastPoint(Vec2& lastPointOut) const
{
    if (points.size() <= 0) return false;
    lastPointOut = points[points.size() - 1];
    return true;
}

shared_ptr<Plottable> Bezier::copy() const
{
    shared_ptr<Bezier> newItem = make_shared<Bezier>(this->points);
    return static_pointer_cast<Plottable>(newItem);
}

vector<Vec2> Bezier::getPlottablePoints(const SampleSettings& sampleSettings) const
{
    double roughLengthEstimate = 0.0;
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        roughLengthEstimate += points[i].distance(points[i + 1]);
    }
    const int steps = max((int) ceil(roughLengthEstimate * sampleSettings.resolution), 10);
    
    vector<Vec2> outPoints = { points[0] };
    outPoints.reserve((VINDEX(outPoints))steps + 1);
    for (int s = 1; s < steps; s++)
    {
        double factor = s / (double)steps;
        vector<Vec2> bezierPoints = points;
        for (int maxIndex = (int)bezierPoints.size() - 1; maxIndex >= 0; maxIndex--)
        {
            for (VINDEX(bezierPoints) i = 0; i < (VINDEX_TYPE(Vec2))maxIndex; i++)
            {
                bezierPoints[i] = bezierPoints[i].moveFactor(bezierPoints[i + 1], factor);
            }
        }
        outPoints.push_back(bezierPoints[0]);
    }
    outPoints.push_back(points[points.size() - 1]);
    
    if (sampleSettings.useStartPoint
        && sampleSettings.startPoint.distanceSquared(outPoints[outPoints.size() - 1]) < sampleSettings.startPoint.distanceSquared(outPoints[0]))
    {
        std::reverse(begin(outPoints), end(outPoints));
    }
    return outPoints;
}

bool Bezier::surrounds(const Vec2& pointToCheck) const
{
    const Rectangle& bounds = this->getBounds();
    const Vec2 startPoint = Vec2(bounds.bottomLeft.x - 1.0, pointToCheck.y);

    return Geometry::getAllIntersections(startPoint, pointToCheck, this).size() % 2 == 1;
}

double Bezier::getLength() const
{
    vector<Vec2> plottablePoints = getPlottablePoints(SampleSettings(15.0));
    if (plottablePoints.size() <= 1) return 0.0;
    double length = 0;
    for (VINDEX(plottablePoints) i = 0; i < plottablePoints.size() - 1; i++)
    {
        length += plottablePoints[i].distance(points[i + 1]);
    }
    return length;
}

double Bezier::getArea() const
{
    vector<Vec2> plottablePoints = getPlottablePoints(SampleSettings(15.0));
    double area = 0.0;
    if (plottablePoints.size() == 0)
    {
        return 0.0;
    }
  
    VINDEX(plottablePoints) j = plottablePoints.size() - 1;
    for (VINDEX(plottablePoints) i = 0; i < plottablePoints.size(); i++)
    {
        area += (plottablePoints[j].x + plottablePoints[i].x) * (plottablePoints[j].y - plottablePoints[i].y);
        j = i;
    }
  
    return abs(area / 2);
}

Vec2 Bezier::getCenterPointsAverage() const
{
    Vec2 average = Vec2(0, 0);
    VINDEX(points) numPoints = points.size();
    if (getIsClosed()) numPoints--;
    for (VINDEX(points) i = 0; i < numPoints; i++)
    {
        average += points[i] / numPoints;
    }
    return average;
}

bool Bezier::getIsClosed() const
{
    if (points.size() <= 1) return false;
    return points[0].isNearly(points[points.size() - 1]);
}



void Bezier::serialize(ostream& os) const
{
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        os << points[i];
        if (i != points.size() - 1)
        {
            os << splitter;
        }
    }
}
shared_ptr<Serializable> Bezier::deserialize(const vector<string>& subElements)
{
    shared_ptr<Bezier> newBezier = make_shared<Bezier>();
    for (const string& element : subElements)
    {
        shared_ptr<Serializable> pointSer = SerializableFactory::createPtr(element);
        newBezier->add(*dynamic_cast<Vec2*>(pointSer.get()));
    }
    return newBezier;
}
string Bezier::getName() const
{
    return "be";
}

void Bezier::offset(const Vec2& dist)
{
    for (Vec2& point : this->points)
    {
        point += dist;
    }
};

void Bezier::scale(const double factor)
{
    for (Vec2& point : this->points)
    {
        point *= factor;
    }
}

void Bezier::scale2D(const Vec2& factor)
{
    for (Vec2& point : this->points)
    {
        point *= factor;
    }
}

void Bezier::scale2DAt(const Vec2& pivot, const Vec2& factor)
{
    this->offset(pivot * -1.0);
    this->scale2D(factor);
    this->offset(pivot);
}

void Bezier::flipX(double x)
{
    for (Vec2& point : points)
    {
        point = point.flipX(x);
    }
}
void Bezier::flipY(double y)
{
    for (Vec2& point : points)
    {
        point = point.flipY(y);
    }
}

bool Bezier::canBeRotated() const
{
    return true;
}
void Bezier::rotate(const Vec2& pivot, double angle)
{
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        points[i] = points[i].rotate(pivot, angle);
    }
}

}
