#pragma once
#include "stl.hpp"

namespace otter { class Queue; }

namespace otter {

/// helper class to export and import .svg files
class SVG {
public:
    /// read an svg and turn it into an otter Queue
    /// @param path path to the .svg file
    /// @param outQueue returned queue
    /// @return success status
    static bool read(const std::string& path, Queue& outQueue);
    /// write an otter Queue to file as .svg file
    /// @param queue queue to convert
    /// @param path path to write to
    /// @param scale factor to scale everything by in .svg coorindates
    /// @param strokeWidth stroke width for lines
    /// @return success status
    static bool write(const Queue& queue, const std::string& path, double scale = 10.0, double strokeWidth = 3.0);
};

}
