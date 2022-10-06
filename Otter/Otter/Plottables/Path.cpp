#include "Path.hpp"
#include "../Math/Geometry.hpp"
#include "Rectangle.hpp"
#include "../Serialize/Serializable.hpp"

using namespace std;

namespace otter {

Path::Path()
    : points({})
{
}

Path::Path(const shared_ptr<Path>& other)
    : points(other->points)
{
}

Path::Path(const vector<Vec2>& points)
{
    this->points = points;
}


shared_ptr<Path> Path::create()
{
    return make_shared<Path>();
}

shared_ptr<Path> Path::create(const shared_ptr<Path>& other)
{
    return make_shared<Path>(other);
}

shared_ptr<Path> Path::create(const vector<Vec2>& points)
{
    return make_shared<Path>(points);
}

shared_ptr<Path> Path::createPolygon(const Vec2& center, int corners, double radius, double rotation)
{
    shared_ptr<Path> path = make_shared<Path>();
    for (int i = 0; i < corners; i++)
    {
        const double angle = rotation + i * (2*M_PI / corners);
        path->add(center + Vec2::polar(angle, radius));
    }
    path->close();
    return path;
}



void Path::add(const Vec2& newPoint)
{
    this->points.push_back(newPoint);
}

void Path::add(const vector<Vec2>& newPoints)
{
    for (const Vec2& point : newPoints)
    {
        add(point);
    }
}

void Path::add(const Path& pathToAppend, bool reversed)
{
    for (VINDEX(pathToAppend.points) i = 0; i < pathToAppend.points.size(); i++)
    {
        if (reversed)
        {
            add(pathToAppend.points[pathToAppend.points.size() - i - 1]);
        }
        else
        {
            add(pathToAppend.points[i]);
        }
    }
}
void Path::add(const shared_ptr<Path>& pathToAppend, bool reversed)
{
    for (VINDEX(pathToAppend->points) i = 0; i < pathToAppend->points.size(); i++)
    {
        if (reversed)
        {
            add(pathToAppend->points[pathToAppend->points.size() - i - 1]);
        }
        else
        {
            add(pathToAppend->points[i]);
        }
    }
}

void Path::addFront(Vec2 newPoint)
{
    this->points.insert(this->points.begin(), newPoint);
}
void Path::addFront(const Path& pathToAppend, bool reversed)
{
    for (VINDEX(pathToAppend.points) i = 0; i < pathToAppend.points.size(); i++)
    {
        if (reversed)
        {
            addFront(pathToAppend.points[i]);
        }
        else
        {
            addFront(pathToAppend.points[pathToAppend.points.size() - i - 1]);
        }
    }
}
void Path::addFront(const shared_ptr<Path>& pathToAppend, bool reversed)
{
    for (VINDEX(pathToAppend->points) i = 0; i < pathToAppend->points.size(); i++)
    {
        if (reversed)
        {
            addFront(pathToAppend->points[i]);
        }
        else
        {
            addFront(pathToAppend->points[pathToAppend->points.size() - i - 1]);
        }
    }
}



void Path::reverse()
{
    std::reverse(points.begin(), points.end());
}

void Path::close()
{
    if (!getIsClosed() && points.size() > 1)
    {
        this->points.push_back(points[0]);
    }
}

bool Path::getLastPoint(Vec2& lastPointOut) const
{
    if (points.size() <= 0) return false;
    lastPointOut = points[points.size() - 1];
    return true;
}


shared_ptr<Plottable> Path::copy() const
{
    shared_ptr<Path> newItem = make_shared<Path>(this->points);
    return static_pointer_cast<Plottable>(newItem);
}

vector<Vec2> Path::getPlottablePoints(const SampleSettings& sampleSettings) const
{
    vector<Vec2> output = points;
    if (output.size() <= 0) return output;
    
    if (sampleSettings.useStartPoint
        && sampleSettings.startPoint.distanceSquared(output[output.size() - 1]) < sampleSettings.startPoint.distanceSquared(output[0]))
    {
        std::reverse(begin(output), end(output));
    }
    return { output };
}

bool Path::surrounds(const Vec2& pointToCheck) const
{
    const Rectangle& bounds = this->getBounds();
    const Vec2 startPoint = Vec2(bounds.bottomLeft.x - 1.0, pointToCheck.y);
    
    return Geometry::getAllIntersections(startPoint, pointToCheck, this).size() % 2 == 1;
}

double Path::getLength() const
{
    if (points.size() <= 1) return 0.0;
    double length = 0;
    for (VINDEX(points) i = 0; i < points.size() - 1; i++)
    {
        length += points[i].distance(points[i + 1]);
    }
    return length;
}

double Path::getArea() const
{
    double area = 0.0;
    if (points.size() == 0)
    {
        return 0.0;
    }
  
    VINDEX(points) j = points.size() - 1;
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
        j = i;
    }
  
    return abs(area / 2);
}

Vec2 Path::getCenterPointsAverage() const
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

bool Path::getIsClosed() const
{
    if (points.size() <= 1) return false;
    return points[0].isNearly(points[points.size() - 1]);
}



void Path::serialize(ostream& os) const
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
shared_ptr<Serializable> Path::deserialize(const vector<string>& subElements)
{
    shared_ptr<Path> newPath = make_shared<Path>();
    for (const string& element : subElements)
    {
        shared_ptr<Serializable> pointSer = SerializableFactory::createPtr(element);
        newPath->add(*dynamic_cast<Vec2*>(pointSer.get()));
    }
    return newPath;
}
string Path::getName() const
{
    return "pa";
}

void Path::offset(const Vec2& dist)
{
    for (Vec2& point : this->points)
    {
        point += dist;
    }
};

void Path::scale(const double factor)
{
    for (Vec2& point : this->points)
    {
        point *= factor;
    }
}

void Path::scale2D(const Vec2& factor)
{
    for (Vec2& point : this->points)
    {
        point *= factor;
    }
}

void Path::scale2DAt(const Vec2& pivot, const Vec2& factor)
{
    this->offset(pivot * -1.0);
    this->scale2D(factor);
    this->offset(pivot);
}

void Path::flipX(double x)
{
    for (Vec2& point : points)
    {
        point = point.flipX(x);
    }
}
void Path::flipY(double y)
{
    for (Vec2& point : points)
    {
        point = point.flipY(y);
    }
}

bool Path::canBeRotated() const
{
    return true;
}
void Path::rotate(const Vec2& pivot, double angle)
{
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        points[i] = points[i].rotate(pivot, angle);
    }
}

}
