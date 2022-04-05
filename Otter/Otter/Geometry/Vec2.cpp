#include "Vec2.hpp"
#include "../Rand/Rand.hpp"
#include "../Math/Math.hpp"
#include "Line.hpp"
#include "Vec2i.hpp"
#include "../Plottables/Circle.hpp"

using namespace std;

namespace otter {

const Vec2 Vec2::a7 = Vec2( 7.4,  10.5);
const Vec2 Vec2::a6 = Vec2(10.5,  14.8);
const Vec2 Vec2::a5 = Vec2(14.8,  21.0);
const Vec2 Vec2::a4 = Vec2(21.0,  29.7);
const Vec2 Vec2::a3 = Vec2(29.7,  42.0);
const Vec2 Vec2::a2 = Vec2(42.0,  59.4);
const Vec2 Vec2::a1 = Vec2(59.4,  84.1);
const Vec2 Vec2::a0 = Vec2(84.1, 118.9);

Vec2::Vec2()
    : x(0.0)
    , y(0.0)
{
}
Vec2::Vec2(double x, double y)
    : x(x)
    , y(y)
{
}
Vec2::Vec2(const Vec2i& vec2i)
    : Vec2((double)vec2i.x, (double)vec2i.y)
{
}

Vec2 Vec2::xy(double xAndY)
{
    return Vec2(xAndY, xAndY);
}
Vec2 Vec2::polar(double angle, double distance)
{
    return Vec2(cos(angle) * distance, sin(angle) * distance);
}
Vec2 Vec2::random(Vec2 bottomLeft, Vec2 topRight)
{
    return Vec2(Rand::randF(bottomLeft.x, topRight.x), Rand::randF(bottomLeft.y, topRight.y));
}

Vec2i Vec2::round() const
{
    return Vec2i::round(*this);
}

Vec2i Vec2::floor() const
{
    return Vec2i::floor(*this);
}

Vec2i Vec2::ceil() const
{
    return Vec2i::ceil(*this);
}

Vec2 Vec2::moveDist(const Vec2& to, double length) const
{
    const double distance = this->distance(to);
    return *this + (to - *this) * (length / distance);
}
Vec2 Vec2::moveFactor(const Vec2& to, double factor) const
{
    return *this + (to - *this) * factor;
}

Vec2 Vec2::normalLeft() const
{
    return Vec2(this->y * -1.0, this->x);
}

Vec2 Vec2::rotate(double angle) const
{
    const double cs = cos(angle);
    const double sn = sin(angle);
    return Vec2(x * cs - y * sn,
                x * sn + y * cs);
}
Vec2 Vec2::rotate(const Vec2& pivot, double angle) const
{
    return this->offset(pivot * -1.0)
        .rotate(angle)
        .offset(pivot);
}

Vec2 Vec2::offset(const Vec2& direction) const
{
    return *this + direction;
}
Vec2 Vec2::offsetX(double xDirection) const
{
    return Vec2(this->x + xDirection, this->y);
}
Vec2 Vec2::offsetY(double yDirection) const
{
    return Vec2(this->x, this->y + yDirection);
}


Vec2 Vec2::normalize() const
{
    const double length = getLength();
    return Vec2(x / length, y / length);
}
Vec2 Vec2::normalize(double length) const
{
    const double factor = length / getLength();
    return Vec2(x * factor, y * factor);
}

Vec2 Vec2::scale(double factor) const
{
    return *this * Vec2::xy(factor);
}
Vec2 Vec2::scale(const Vec2& factor) const
{
    return *this * factor;
}
Vec2 Vec2::scale(const Vec2& center, double factor) const
{
    return this->offset(center * -1.0)
        .scale(factor)
        .offset(center);
}

Vec2 Vec2::swapAxes() const
{
    return Vec2(y, x);
}
Vec2 Vec2::flipSign() const
{
    return Vec2(x * -1.0, y * -1.0);
}
Vec2 Vec2::flipSignX() const
{
    return Vec2(x * -1.0, y);
}
Vec2 Vec2::flipSignY() const
{
    return Vec2(x, y * -1.0);
}
Vec2 Vec2::flipX(double atX) const
{
    return Vec2(atX + atX - x, y);
}
Vec2 Vec2::flipY(double atY) const
{
    return Vec2(x, atY + atY - y);
}

Vec2 Vec2::onlyX() const
{
    return Vec2(x, 0.0);
}
Vec2 Vec2::onlyY() const
{
    return Vec2(0.0, y);
}


Vec2 Vec2::invert(const shared_ptr<Circle>& inversionC) const
{
    return inversionC->center + (pow(inversionC->radius, 2.0) * (*this - inversionC->center)) / pow(this->distance(inversionC->center), 2.0);
}




double Vec2::getLength() const
{
    return sqrt(pow(this->x, 2.0) + pow(this->y, 2.0));
}
double Vec2::getAngle(bool onlyPositive) const
{
    double angle = atan2(this->y, this->x);
    if (onlyPositive && angle < 0.0) return angle + M_PI * 2.0;
    return angle;
}

double Vec2::distance(const Vec2& other) const
{
    return sqrt(pow(other.x - this->x, 2.0) + pow(other.y - this->y, 2.0));
}

double Vec2::distanceSquared(const Vec2& other) const
{
    return pow(other.x - this->x, 2.0) + pow(other.y - this->y, 2.0);
}

double Vec2::distanceManhattan(const Vec2& other) const
{
    return abs(other.x - this->x) + abs(other.y - this->y);
}

bool Vec2::isNearly(const Vec2& other, double delta) const
{
    return distanceManhattan(other) < delta;
}

bool Vec2::isLeftOfLine(const Line& line) const
{
    return (line.b - line.a).cross(*this - line.a) > 0.0;
}

bool Vec2::isOnLine(const Line& line) const
{
    return abs((line.b - line.a).cross(*this - line.a)) <= 0.000001;
}

bool Vec2::isFacingLessThan90Off(const Vec2& other) const
{
    return dot(other) > 0.0;
}

double Vec2::dot(const Vec2& other) const
{
    return x * other.x + y * other.y;
}

double Vec2::cross(const Vec2& other) const
{
    return x * other.y - y * other.x;
}

double Vec2::min() const
{
    return std::min(x, y);
}
double Vec2::max() const
{
    return std::max(x, y);
}


std::complex<double> Vec2::toComplex() const
{
    return std::complex<double>(x, y);
}
Vec2 Vec2::fromComplex(const std::complex<double>& complexNumber)
{
    return Vec2(complexNumber.real(), complexNumber.imag());
}


void Vec2::serialize(std::ostream& os) const
{
    os << x << splitter << y;
}
std::shared_ptr<Serializable> Vec2::deserialize(const std::vector<std::string>& subElements)
{
    if (subElements.size() != 2)
    {
        std::cout << "ERROR: Vec2 deserialize got " << subElements.size() << " elements:" << std::endl;
        for (const std::string& element : subElements)
        {
            std::cout << "\t" << element << std::endl;
        }
        return nullptr;
    }
    
    return std::make_shared<Vec2>(Math::doubleFromString(subElements[0]), Math::doubleFromString(subElements[1]));
}
std::string Vec2::getName() const
{
    return "v2";
}



Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(this->x + other.x, this->y + other.y);
}
Vec2 Vec2::operator+(const double& other) const
{
    return Vec2(this->x + other, this->y + other);
}
void Vec2::operator+=(const Vec2& other)
{
    this->x += other.x;
    this->y += other.y;
}
void Vec2::operator+=(const double& other)
{
    this->x += other;
    this->y += other;
}
Vec2 operator+(const double& a, const Vec2& b)
{
    return Vec2(a + b.x, a + b.y);
}


Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(this->x - other.x, this->y - other.y);
}
Vec2 Vec2::operator-(const double& other) const
{
    return Vec2(this->x - other, this->y - other);
}
void Vec2::operator-=(const Vec2& other)
{
    this->x -= other.x;
    this->y -= other.y;
}
void Vec2::operator-=(const double& other)
{
    this->x -= other;
    this->y -= other;
}
Vec2 operator-(const double& a, const Vec2& b)
{
    return Vec2(a - b.x, a - b.y);
}


Vec2 Vec2::operator*(const Vec2& other) const
{
    return Vec2(this->x * other.x, this->y * other.y);
}
Vec2 Vec2::operator*(const double& other) const
{
    return Vec2(this->x * other, this->y * other);
}
void Vec2::operator*=(const Vec2& other)
{
    this->x *= other.x;
    this->y *= other.y;
}
void Vec2::operator*=(const double& other)
{
    this->x *= other;
    this->y *= other;
}
Vec2 operator*(const double& a, const Vec2& b)
{
    return Vec2(a * b.x, a * b.y);
}


Vec2 Vec2::operator/(const Vec2& other) const
{
    return Vec2(this->x / other.x, this->y / other.y);
}
Vec2 Vec2::operator/(const double& other) const
{
    return Vec2(this->x / other, this->y / other);
}
void Vec2::operator/=(const Vec2& other)
{
    this->x /= other.x;
    this->y /= other.y;
}
void Vec2::operator/=(const double& other)
{
    this->x /= other;
    this->y /= other;
}
Vec2 operator/(const double& a, const Vec2& b)
{
    return Vec2(a / b.x, a / b.y);
}



bool Vec2::operator==(const Vec2& other) const
{
    return this->isNearly(other);
}
bool Vec2::operator!=(const Vec2& other) const
{
    return !this->isNearly(other);
}

}
