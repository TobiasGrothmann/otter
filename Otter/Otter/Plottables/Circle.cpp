#include "Circle.hpp"
#include "Rectangle.hpp"
#include "../Serialize/Serializable.hpp"
#include "../Math/Math.hpp"

using namespace std;

namespace otter {

Circle::Circle(const shared_ptr<Circle>& other)
    : center(other->center)
    , radius(other->radius)
{
}
Circle::Circle(const Vec2& center, double radius)
    : center(center)
    , radius(radius)
{}


shared_ptr<Circle> Circle::create(const shared_ptr<Circle>& other)
{
    return make_shared<Circle>(other);
}
shared_ptr<Circle> Circle::create(const Vec2& center, double radius)
{
    return make_shared<Circle>(center, radius);
}

shared_ptr<Plottable> Circle::copy() const
{
    shared_ptr<Circle> newItem = make_shared<Circle>(this->center, this->radius);
    return static_pointer_cast<Plottable>(newItem);
}

vector<Vec2> Circle::getPlottablePoints(const SampleSettings& sampleSettings) const
{
    const int n = (int)max(abs(getLength()) * sampleSettings.resolution, 12.0);
    const double anglePerStep = (2.0 * M_PI) / n;
    double startAngle = 0.0;
    if (sampleSettings.useStartPoint)
    {
        startAngle = (sampleSettings.startPoint - this->center).getAngle(true);
    }
    
    vector<Vec2> output = vector<Vec2>();
    output.reserve((VINDEX(output))n + 1);
    for (int i = 0; i < n + 1; i++) {
        output.push_back(this->center + Vec2::polar(startAngle + i * anglePerStep, this->radius));
    }
    return output;
}

bool Circle::surrounds(const Vec2& pointToCheck) const
{
    return this->center.distance(pointToCheck) < abs(this->radius);
}

double Circle::getLength() const
{
    return M_PI * 2.0 * abs(radius);
}
double Circle::getArea() const
{
    return M_PI * radius * radius;
}
Vec2 Circle::getCenterPointsAverage() const
{
    return center;
}
bool Circle::getIsClosed() const
{
    return true;
}

Rectangle Circle::getBounds() const
{
    return Rectangle(center - abs(radius), center + abs(radius));
}

void Circle::serialize(ostream& os) const
{
    os << center << splitter << to_string(radius);
}
shared_ptr<Serializable> Circle::deserialize(const vector<string>& subElements)
{
    shared_ptr<Serializable> newCenterShared = SerializableFactory::createPtr(subElements[0]);
    Vec2* newCenter = dynamic_pointer_cast<Vec2>(newCenterShared).get();
    
    const double newRadius = Math::doubleFromString(subElements[1]);
    
    return make_shared<Circle>(*newCenter, newRadius);
}
string Circle::getName() const
{
    return "ci";
}

void Circle::offset(const Vec2& dist)
{
    center += dist;
};

void Circle::scale(const double factor)
{
    center *= factor;
    radius *= factor;
}

void Circle::flipX(double x)
{
    center = center.flipX(x);
}
void Circle::flipY(double y)
{
    center = center.flipY(y);
}

bool Circle::canBeRotated() const
{
    return true;
}
void Circle::rotate(const Vec2& pivot, double angle)
{
    center = center.rotate(pivot, angle);
}

shared_ptr<Plottable> Circle::GetInvertedInCircle(const shared_ptr<Circle>& inversionCircle, const SampleSettings&) const
{
    const double s = pow(inversionCircle->radius, 2.0) / (pow(this->center.x - inversionCircle->center.x, 2.0) + pow(this->center.y - inversionCircle->center.y, 2.0) - pow(this->radius, 2.0));
    const double newX = inversionCircle->center.x + s * (this->center.x - inversionCircle->center.x);
    const double newY = inversionCircle->center.y + s * (this->center.y - inversionCircle->center.y);
    const double newRadius = abs(s) * this->radius;
    return Circle::create(Vec2(newX, newY), newRadius);
}

}
