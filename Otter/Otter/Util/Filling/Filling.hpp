#pragma once
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Plottable.hpp"

namespace otter {

/// helper class to generate pen strokes that fill a shape on paper with a regular pen
class Filling
{
public:
    /// generates filling with horizontal lines that are not connected to each other. This results in a lot of pen lifting and dropping.
    /// @param shape the shape to be filled; expected to be closed to get useful results
    /// @param lineDist distance of the lines from each other; should be the width of the pen or smaller.
    static Queue getSimpleFilling(const std::shared_ptr<Plottable>& shape, double lineDist);
};

}

