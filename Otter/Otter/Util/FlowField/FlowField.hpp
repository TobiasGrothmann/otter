#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Path.hpp"

namespace otter {

/// FlowField defines a custom mapping for each point on the plane to another point.
/// The field-function should be pure, in the sense that it should not have any side effects and return the same new vector for a given vector every time.
/// The function is also intended for small incremental changes of the original point
///
/// perlin noise flow field:
/// \code{.cpp}
/// Queue q = Queue();
/// Rand::seed();
///
/// FlowField field = FlowField([](const Vec2& originalPoint) -> Vec2
/// {
///     const double angle = Rand::perlin(originalPoint * 0.1) * 2.0 * M_PI;
///     return originalPoint + Vec2::polar(angle, 0.05);
/// });
///
/// HexGridVertices hexGrid = HexGridVertices(0.5, true);
/// const vector<Vec2> hexPoints = hexGrid.create(Rectangle::create(Vec2(0, 0), Vec2(20, 20)));
/// for (const Vec2& hexPoint : hexPoints)
/// {
///     PathPtr p = field.walk(hexPoint, 2.0);
///     q.add(p);
/// }
/// \endcode
class FlowField
{
public:
    FlowField(std::function<Vec2(const Vec2&)> function);
    
    Vec2 sampleAt(const Vec2& point) const;
    std::shared_ptr<Path> walk(const Vec2& start, double length) const;

private:
    std::function<Vec2(const Vec2&)> function;
};

}
