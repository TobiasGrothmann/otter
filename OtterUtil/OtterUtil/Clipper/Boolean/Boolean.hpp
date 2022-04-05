#pragma once
#include "stl.hpp"
#include "Otter.hpp"
#include "../../Clipper/ClipperConvert/ClipperConvert.hpp"

namespace otter {

/// wrapper class for clipper's offset
class Boolean
{
public:
    static Queue getIntersection(const PlottablePtr& a, const PlottablePtr& b);
    static Queue getUnion(const PlottablePtr& a, const PlottablePtr& b);
    static Queue getANotB(const PlottablePtr& a, const PlottablePtr& b);
    static Queue getBNotA(const PlottablePtr& a, const PlottablePtr& b);
    static Queue getXor(const PlottablePtr& a, const PlottablePtr& b);
    
    static Queue getIntersection(const Queue& a, const Queue& b);
    static Queue getUnion(const Queue& a, const Queue& b);
    static Queue getANotB(const Queue& a, const Queue& b);
    static Queue getBNotA(const Queue& a, const Queue& b);
    static Queue getXor(const Queue& a, const Queue& b);
    
    static Queue getIntersection(const PlottablePtr& a, const Queue& b);
    static Queue getUnion(const PlottablePtr& a, const Queue& b);
    static Queue getANotB(const PlottablePtr& a, const Queue& b);
    static Queue getBNotA(const PlottablePtr& a, const Queue& b);
    static Queue getXor(const PlottablePtr& a, const Queue& b);
    
    static Queue getIntersection(const Queue& a, const PlottablePtr& b);
    static Queue getUnion(const Queue& a, const PlottablePtr& b);
    static Queue getANotB(const Queue& a, const PlottablePtr& b);
    static Queue getBNotA(const Queue& a, const PlottablePtr& b);
    static Queue getXor(const Queue& a, const PlottablePtr& b);
    
private:
    static Queue execute(const std::vector<PlottablePtr>& a, const std::vector<PlottablePtr>& b, ClipperLib::ClipType type);
};

}
