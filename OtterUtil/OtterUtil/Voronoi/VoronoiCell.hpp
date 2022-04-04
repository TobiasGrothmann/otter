#pragma once
#include "Otter.hpp"

namespace otter {

/// single cell of voronoi cell
class VoronoiCell
{
public:
    VoronoiCell(const Vec2& point, const std::vector<Line>& edges, const std::vector<Vec2>& vertices);
    
    /// create path around edges of cell
    std::shared_ptr<Path> createPath() const;
    
    const Vec2 point; ///< vertex
    const std::vector<Line> edges; ///< lines of all edges to neighbouring cells
    const std::vector<Vec2> vertices; ///< vertices of all corner points for all edges of neighbouring cells
};

}
