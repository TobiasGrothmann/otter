#pragma once
#include "Otter.hpp"
#include "Voronoi.hpp"

namespace otter {

/// computes voronoi diagram with varied densities defined by a closure
class VoronoiFunction
{
public:
    /// create a voronoi diagram, and iterates it to conform the density with the given closure
    /// @param function closure that defines the densitiy
    /// @param clipBounds bounds to clip diagram in
    /// @param iterations number of iterations for the generation, the more, the more exact it will get
    /// @param moveToCentroidFactor factor by which points get nudged to the direction they need to go
    static Voronoi create(std::function<double(const Vec2& point)> function,
                          std::shared_ptr<Rectangle> clipBounds,
                          int iterations = 20,
                          double moveToCentroidFactor = 1.0);
    
private:
    static std::pair<Vec2, Vec2> getSplitCell(const VoronoiCell& cell);
    static PathPtr getDot(const Vec2& point);
    static Vec2 getWeightedAverage(const VoronoiCell& cell, std::function<double(const Vec2& point)> func);
};

}
