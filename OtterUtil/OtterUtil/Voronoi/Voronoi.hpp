#pragma once
#include "Otter.hpp"
#include "VoronoiCell.hpp"

namespace otter {

class Voronoi
{
public:
    Voronoi() = delete;
    Voronoi(const std::vector<Vec2>& points, std::shared_ptr<Rectangle> clipBounds = nullptr);
    
    std::vector<Line> edges = {};
    std::vector<VoronoiCell> cells = {};
    
private:
    const std::vector<Vec2> points;
    const std::shared_ptr<Rectangle> clipBounds;
};

}
