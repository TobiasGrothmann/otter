#pragma once
#include "Otter.hpp"
#include "VoronoiCell.hpp"

namespace otter {

/// computes voronoi diagram using jc_voronoi from list of points inside of clipBounds
class Voronoi
{
public:
    Voronoi() = delete;
    /// create a voronoi diagram for the given points inside of the rectangle clipBounds
    /// @param points voronoi vertices
    /// @param clipBounds bounds to clip diagram in
    Voronoi(const std::vector<Vec2>& points, std::shared_ptr<Rectangle> clipBounds = nullptr);
    
    std::vector<Line> edges = {}; ///< edges between all cells and towards infinity (clipped ad clipBounds)
    std::vector<VoronoiCell> cells = {};
    
private:
    const std::vector<Vec2> points;
    const std::shared_ptr<Rectangle> clipBounds;
};

}
