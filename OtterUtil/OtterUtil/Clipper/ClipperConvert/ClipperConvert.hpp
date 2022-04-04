#pragma once
#include "stl.hpp"
#include "Otter.hpp"
#include "../clipper/clipper.hpp"

#define CLIPPER_RESOLUTION 2000.0

namespace otter {

/// helper class for conversion from otter types to clipper types and reverse;
/// clipper uses integer values while otter uses floating point, so values are being converted/scaled; CLIPPER_RESOLUTION is the amount of resolution used for that.
class ClipperConvert
{
public:
    static ClipperLib::cInt toClipperCoord(double value);
    static double fromClipperCoord(ClipperLib::cInt value);
    
    static ClipperLib::IntPoint intPointFromVec2(const Vec2& point);
    static Vec2 vec2FromIntPoint(const ClipperLib::IntPoint point);
    
    static ClipperLib::Path pathFromPlottable(const std::shared_ptr<Plottable>& plottable);
    static std::shared_ptr<Path> plottableFromPath(const ClipperLib::Path path);
    
    static ClipperLib::Paths pathsFromQueue(const Queue& queue);
    static Queue queueFromPaths(const ClipperLib::Paths& paths);
};

}
