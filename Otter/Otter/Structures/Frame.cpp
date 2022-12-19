#include "Frame.hpp"

using namespace std;

namespace otter {

Frame::Frame(const Vec2& bottomLeft, const Vec2& size, const Vec2& margin)
    : bottomLeft(bottomLeft)
    , size(size)
    , margin(margin)
{
}
Frame::Frame(const Vec2& bottomLeft, const Vec2& size, double margin)
    : Frame(bottomLeft, size, Vec2::xy(margin))
{
}
Frame::Frame(const Vec2& size, const Vec2& margin)
    : Frame(Vec2(0.0, 0.0), size, margin)
{
}
Frame::Frame(const Vec2& size, double margin)
    : Frame(Vec2(0.0, 0.0), size, Vec2::xy(margin))
{
}

shared_ptr<Rectangle> Frame::getInnerRectangle() const
{
    return make_shared<Rectangle>(bottomLeft + margin, bottomLeft + size - margin);
}

shared_ptr<Rectangle> Frame::getOuterRectangle() const
{
    return make_shared<Rectangle>(bottomLeft, bottomLeft + size);
}

otter::Vec2 Frame::getCenter() const
{
    return bottomLeft + size * 0.5;
}

}
