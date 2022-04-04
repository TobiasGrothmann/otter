#pragma once
#include "stl.hpp"
#include "Otter.hpp"
#include "../../Clipper/ClipperConvert/ClipperConvert.hpp"

namespace otter {

/// helper class to create offset to plottable using the clipper library
class Offset
{
public:
    /// join type for corners
    enum class JoinType
    {
        /// squared corners
        square,
        /// rounded arc corners
        round,
        /// cut off corners
        miter
    };
    
    /// end type for offset
    enum class EndType
    {
        /// squared ends
        square,
        /// rounded half circle ends
        round,
        /// flat ends
        butt
    };
    
    /// create offset to plottable
    /// @param plottable original shape
    /// @param distance distance to offset by
    /// @param joinType join type for corners
    /// @param endType end type for start and end
    static Queue getOffset(const PlottablePtr& plottable,
                           double distance,
                           JoinType joinType = JoinType::square,
                           EndType endType = EndType::butt);
    
private:
    static ClipperLib::JoinType convertJoinType(JoinType joinType);
    static ClipperLib::EndType convertEndType(EndType endType);
};

}
