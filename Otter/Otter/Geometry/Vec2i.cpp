#include "Vec2i.hpp"
#include "Vec2.hpp"

using namespace std;

namespace otter {

Vec2i::Vec2i()
    : x(0)
    , y(0)
{
}

Vec2i::Vec2i(int x, int y)
    : x(x)
    , y(y)
{
}

Vec2i Vec2i::round(const Vec2& vec2)
{
    return Vec2i((int)std::round(vec2.x), (int)std::round(vec2.y));
}

Vec2i Vec2i::ceil(const Vec2& vec2)
{
    return Vec2i((int)std::ceil(vec2.x), (int)std::ceil(vec2.y));
}

Vec2i Vec2i::floor(const Vec2& vec2)
{
    return Vec2i((int)std::floor(vec2.x), (int)std::floor(vec2.y));
}

}

