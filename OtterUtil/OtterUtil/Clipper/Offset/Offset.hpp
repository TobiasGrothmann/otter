#pragma once
#include "stl.hpp"
#include "Otter.hpp"
#include "../../Clipper/ClipperConvert/ClipperConvert.hpp"

namespace otter {

class Offset
{
public:
    enum class JoinType
    {
        square,
        round,
        miter
    };

    enum class EndType
    {
        square,
        round,
        butt
    };
    
    static Queue getOffset(const PlottablePtr& plottable,
                           double distance,
                           JoinType joinType = JoinType::square,
                           EndType endType = EndType::butt);
    
private:
    static ClipperLib::JoinType convertJoinType(JoinType joinType);
    static ClipperLib::EndType convertEndType(EndType endType);
};

}
