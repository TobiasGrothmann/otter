#pragma once
#include "Otter.hpp"

namespace otter {

class VoronoiCell
{
public:
    VoronoiCell(const Vec2& point, const std::vector<Line>& edges, const std::vector<Vec2>& vertices);
    
    std::shared_ptr<Path> createPath() const;
    
    const Vec2 point;
    const std::vector<Line> edges;
    const std::vector<Vec2> vertices;
};

}
