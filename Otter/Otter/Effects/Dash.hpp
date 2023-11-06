#pragma once
#include "stl.hpp"
#include "../Queue/Queue.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Geometry/Vec2.hpp"

namespace otter {

/// Handles dashing of plottables. Dashing divides the paths into segments and draws only every second segment.
class Dash
{
public:
    /// Dash a single plottable.
    /// @param plottable plottable to dash
    /// @param func function that defines how long the dashes are at each location.
    static Queue dash(const std::shared_ptr<Plottable> plottable,
                      const std::function<double(const Vec2& point,
                                                 bool isGap,
                                                 double pathDistance,
                                                 double pathFraction)>& func);
    
    /// Dash a single plottable.
    /// @param plottable plottable to dash
    /// @param dashLength defines the dash length
    /// @param gapLength defines the gap length
    static Queue dash(const std::shared_ptr<Plottable> plottable,
                      double dashLength,
                      double gapLength);
    
    /// Dash all plottables in a queue.
    /// @param queue queue to dash
    /// @param func function that defines how long the dashes are at each location.
    static Queue dash(const Queue& queue,
                      double penWidth,
                      const std::function<double(const Vec2& point,
                                                 bool isGap,
                                                 double pathDistance,
                                                 double pathFraction)>& func);
    
    /// Dash all plottables in a queue.
    /// @param queue queue to dash
    /// @param dashLength defines the dash length
    /// @param gapLength defines the gap length
    static Queue dash(const Queue& queue,
                      double dashLength,
                      double gapLength);
};

}
