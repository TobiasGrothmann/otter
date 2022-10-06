#include "Rectangle.hpp"
#include "Path.hpp"
#include "../Math/Math.hpp"
#include "../Serialize/Serializable.hpp"

using namespace std;

namespace otter {

Rectangle::Rectangle(const shared_ptr<Rectangle>& other)
    : bottomLeft(other->bottomLeft)
    , topRight(other->topRight)
{
}

Rectangle::Rectangle(const Vec2& bottomLeft, const Vec2& topRight)
{
    this->bottomLeft = Vec2(fmin(bottomLeft.x, topRight.x), fmin(bottomLeft.y, topRight.y));
    this->topRight = Vec2(fmax(bottomLeft.x, topRight.x), fmax(bottomLeft.y, topRight.y));
}


shared_ptr<Rectangle> Rectangle::create(const shared_ptr<Rectangle>& other)
{
    return make_shared<Rectangle>(other);
}

shared_ptr<Rectangle> Rectangle::create(const Vec2& bottomLeft, const Vec2& topRight)
{
    return make_shared<Rectangle>(bottomLeft, topRight);
}


shared_ptr<Plottable> Rectangle::copy() const
{
    shared_ptr<Rectangle> newItem = make_shared<Rectangle>(this->bottomLeft, this->topRight);
    return static_pointer_cast<Plottable>(newItem);
}

Vec2 Rectangle::getSize() const
{
    return topRight - bottomLeft;
}
Vec2 Rectangle::getCenter() const
{
    return bottomLeft + (topRight - bottomLeft) * 0.5;
}
double Rectangle::getHeight() const
{
    return topRight.y - bottomLeft.y;
}
double Rectangle::getWidth() const
{
    return topRight.x - bottomLeft.x;
}

double Rectangle::getAspectRatio() const
{
    return this->getWidth() / this->getHeight();
}

Vec2 Rectangle::normalizePoint(const Vec2& point) const
{
    return (point - bottomLeft) / getSize();
}

bool Rectangle::overlaps(const Rectangle& other) const
{
    return !(bottomLeft.x > other.topRight.x ||
             bottomLeft.y > other.topRight.y ||
             topRight.x <= other.bottomLeft.x ||
             topRight.y <= other.bottomLeft.y);
}

bool Rectangle::isSquare() const
{
    return Math::isNearly(this->getWidth(), this->getHeight());
}


shared_ptr<Path> Rectangle::getTopSide() const
{
    return Path::create({ getTopLeft(), topRight });
}
shared_ptr<Path> Rectangle::getRightSide() const
{
    return Path::create({ topRight, getBottomRight() });
}
shared_ptr<Path> Rectangle::getBottomSide() const
{
    return Path::create({ getBottomRight(), bottomLeft });
}
shared_ptr<Path> Rectangle::getLeftSide() const
{
    return Path::create({ bottomLeft, getTopLeft() });
}

Vec2 Rectangle::getTopMid() const
{
    return getTopLeft().moveFactor(topRight, 0.5);
}
Vec2 Rectangle::getRightMid() const
{
    return topRight.moveFactor(getBottomRight(), 0.5);
}
Vec2 Rectangle::getBottomMid() const
{
    return getBottomRight().moveFactor(bottomLeft, 0.5);
}
Vec2 Rectangle::getLeftMid() const
{
    return bottomLeft.moveFactor(getTopLeft(), 0.5);
}



vector<Vec2> Rectangle::getPlottablePoints(const SampleSettings&) const
{
    // TODO: use closest point if SampleSettings.useStartPoint == true
    const Vec2 size = getSize();
    return
    {
        bottomLeft,
        bottomLeft + Vec2(size.x, 0.0),
        bottomLeft + size,
        bottomLeft + Vec2(0.0, size.y),
        bottomLeft
    };
}

bool Rectangle::surrounds(const Vec2& pointToCheck) const
{
    return pointToCheck.x >= bottomLeft.x &&
        pointToCheck.y >= bottomLeft.y &&
        pointToCheck.x <= topRight.x &&
        pointToCheck.y <= topRight.y;
}

double Rectangle::getLength() const
{
    const Vec2 size = getSize();
    return size.x * 2.0 + size.y * 2.0;
}

double Rectangle::getArea() const
{
    const Vec2 size = getSize();
    return size.x * size.y;
}

Vec2 Rectangle::getCenterPointsAverage() const
{
    return bottomLeft + getSize() * 0.5;
}

bool Rectangle::getIsClosed() const
{
    return true;
}



void Rectangle::serialize(ostream& os) const
{
    os << bottomLeft << splitter << topRight;
}
shared_ptr<Serializable> Rectangle::deserialize(const vector<string>& subElements)
{
    shared_ptr<Serializable> bottomLeftShared = SerializableFactory::createPtr(subElements[0]);
    shared_ptr<Serializable> topRightShared = SerializableFactory::createPtr(subElements[1]);
    
    Vec2* newBottomLeft = dynamic_pointer_cast<Vec2>(bottomLeftShared).get();
    Vec2* newTopRight = dynamic_pointer_cast<Vec2>(topRightShared).get();
    
    return make_shared<Rectangle>(*newBottomLeft, *newTopRight);
}
string Rectangle::getName() const
{
    return "re";
}

void Rectangle::offset(const Vec2& dist)
{
    this->bottomLeft += dist;
    this->topRight += dist;
};

void Rectangle::scale(const double factor)
{
    this->bottomLeft *= factor;
    this->topRight *= factor;
}

void Rectangle::flipX(double x)
{
    Vec2 cornerA = Vec2(bottomLeft).flipX(x);
    Vec2 cornerB = Vec2(topRight).flipX(x);
    this->bottomLeft = Vec2(min(cornerA.x, cornerB.x), min(cornerA.y, cornerB.y));
    this->topRight = Vec2(max(cornerA.x, cornerB.x), max(cornerA.y, cornerB.y));
}
void Rectangle::flipY(double y)
{
    Vec2 cornerA = Vec2(bottomLeft).flipY(y);
    Vec2 cornerB = Vec2(topRight).flipY(y);
    this->bottomLeft = Vec2(min(cornerA.x, cornerB.x), min(cornerA.y, cornerB.y));
    this->topRight = Vec2(max(cornerA.x, cornerB.x), max(cornerA.y, cornerB.y));
}

bool Rectangle::canBeRotated() const
{
    return false;
}

Vec2 Rectangle::getBottomRight() const
{
    return Vec2(topRight.x, bottomLeft.y);
}
Vec2 Rectangle::getTopLeft() const
{
    return Vec2(bottomLeft.x, topRight.y);
}
Vec2 Rectangle::getTopRight() const
{
    return topRight;
}
Vec2 Rectangle::getBottomLeft() const
{
    return bottomLeft;
}

}
