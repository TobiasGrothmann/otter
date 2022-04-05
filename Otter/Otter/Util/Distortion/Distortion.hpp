#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Plottable.hpp"
#include "../../Plottables/Path.hpp"

namespace otter {

/// Distortion defines a custom mapping for each point on the plane to another point.
/// The distortion-function should be pure, in the sense that it should not have any side effects and return the same new vector for a given vector every time.
///
/// example distortion that just moves each point to positive x by 1:
/// \code{.cpp}
/// Queue q = Queue();
/// [...]
/// Distortion d = Distortion([](const Vec2& originalPoint) -> Vec2
/// {
///     return originalPoint + Vec2(1.0, 0.0);
/// });
/// Queue distortedQueue = d.getDistorted(q);
/// \endcode
///
/// distortion around a center
/// \code{.cpp}
/// const Vec2 center = Vec2(5, 5);
/// const Distortion d = Distortion([center](const Vec2& originalPoint) -> Vec2
/// {
///     const double angle = (originalPoint - center).getAngle(true);
///     const double distance = originalPoint.distance(center);
///     const double newAngle = angle + sin(distance * 2) * 0.3;
///     return center + Vec2::polar(newAngle, distance);
/// });
/// \endcode
class Distortion
{
public:
    Distortion() = delete;
    Distortion(std::function<Vec2(const Vec2&)> function);
    
    virtual Queue getDistorted(const Queue& queue,
                               const SampleSettings& sampleSettings = SampleSettings()) const;
    virtual std::shared_ptr<Path> getDistorted(const std::shared_ptr<Plottable>& plottable,
                                               const SampleSettings& sampleSettings = SampleSettings()) const;
    
protected:
    std::function<Vec2(const Vec2&)> function;
};

}
