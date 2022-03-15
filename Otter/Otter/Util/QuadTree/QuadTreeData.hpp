#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"

namespace otter { class Plottable; class Rectangle; }

namespace otter {

struct QuadTreeVec
{
    QuadTreeVec() = default;
    
    QuadTreeVec(const Vec2& point, const std::shared_ptr<Plottable>& plottable)
        : point(point)
        , plottable(plottable)
    {
    };
    
    Vec2 point = Vec2();
    std::shared_ptr<Plottable> plottable = nullptr;
};

class Sector
{
public:
    Sector() = default;
    Sector(const std::shared_ptr<Rectangle>& bounds, VINDEX_TYPE(QuadTreeVec) capacity);
    
    void add(const QuadTreeVec& point);
    
    std::vector<QuadTreeVec> points;
    
    bool divided = false;
    std::shared_ptr<Sector> bl = nullptr;
    std::shared_ptr<Sector> br = nullptr;
    std::shared_ptr<Sector> tl = nullptr;
    std::shared_ptr<Sector> tr = nullptr;
    
    VINDEX_TYPE(QuadTreeVec) capacity = 3;
    std::shared_ptr<Rectangle> bounds = nullptr;
    
private:
    void divide();
};

}
