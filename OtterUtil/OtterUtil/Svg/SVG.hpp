#pragma once
#include "stl.hpp"

namespace otter { class Queue; }

namespace otter {

class SVG {
public:
    static bool read(const std::string& path, Queue& outQueue);
    static bool write(const Queue& queue, const std::string& path, double scale = 10.0, double strokeWidth = 3.0);
};

}
