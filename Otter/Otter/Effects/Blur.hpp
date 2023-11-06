#pragma once
#include "stl.hpp"
#include "../Queue/Queue.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Geometry/Vec2.hpp"

namespace otter {

/// Handles blurring of plottables. Blurring distorts the paths by some noise and ripps them apart into separated stippled dots if the blurring is strong.
class Blur
{
public:
    /// Blur a single plottable.
    /// @param plottable plottable to blur
    /// @param penWidth width of the pen, used to choose the interval at which the path is distorted of stippled
    /// @param func function that defines how far the plottable should be blurred for every location.
    static Queue blur(const std::shared_ptr<Plottable> plottable,
                      double penWidth,
                      const std::function<double(const Vec2& point,
                                                 double pathDistance,
                                                 double pathFraction)>& func);
    
    /// Blur a single plottable.
    /// @param plottable plottable to blur
    /// @param penWidth width of the pen, used to choose the interval at which the path is distorted of stippled
    /// @param blurAmount defines how far the plottable should be blurred by (distance value)
    static Queue blur(const std::shared_ptr<Plottable> plottable,
                      double penWidth,
                      double blurAmount);
    
    /// Blur all plottables in a queue.
    /// @param queue queue to blur
    /// @param penWidth width of the pen, used to choose the interval at which the path is distorted of stippled
    /// @param func function that defines how far the plottables should be blurred for every location.
    static Queue blur(const Queue& queue,
                      double penWidth,
                      const std::function<double(const Vec2& point,
                                                 double pathDistance,
                                                 double pathFraction)>& func);
    
    /// Blur all plottables in a queue.
    /// @param queue queue to blur
    /// @param penWidth width of the pen, used to choose the interval at which the path is distorted of stippled
    /// @param blurAmount defines how far the plottables should be blurred by (distance value)
    static Queue blur(const Queue& queue,
                      double penWidth,
                      double blurAmount);
};

}
