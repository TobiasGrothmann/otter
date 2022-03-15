#include "VoronoiCell.hpp"

using namespace std;

namespace otter {

VoronoiCell::VoronoiCell(const Vec2& point, const vector<Line>& edges, const vector<Vec2>& vertices)
    : point(point)
    , edges(edges)
    , vertices(vertices)
{
}

shared_ptr<Path> VoronoiCell::createPath() const
{
    PathPtr path = Path::create(vertices);
    path->close();
    return path;
}

}
